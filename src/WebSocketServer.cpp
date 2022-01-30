#include "WebSocketServer.h"

#include <log4cplus/loggingmacros.h>


void WebSocketServer::Run(int portNumber_)
{
  std::thread([this, portNumber_]() {
    uWS::App().ws<PerSocketData>("/*",
      {
        //.compression = uWS::DEDICATED_COMPRESSOR_256KB, //DISABLED
        //.maxPayloadLength = 16 * 1024 * 1024, //16 * 1024
        //.idleTimeout = 0, //120
        //.maxBackpressure = 0, //64 * 1024

      .upgrade = [this](auto* res_, auto* req_, auto* context_)
      {
        res_->cork([this, res_, req_, context_]()
        {
          res_->template upgrade<PerSocketData>({
            .connectionId = ++m_connectionCounter
            },
            req_->getHeader("sec-websocket-key"),
            req_->getHeader("sec-websocket-protocol"),
            req_->getHeader("sec-websocket-extensions"),
            context_);
        });
      },
      .open = [this](TWebSocket* webSocket_)
      {
        uint64_t connectionId = webSocket_->getUserData()->connectionId;
        LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), "Open connection id: " << connectionId);

        m_webSocketWorkers.emplace(connectionId, std::make_unique<WebSocketWorker>());
        m_webSocketWorkers[connectionId]->Initialize(connectionId, webSocket_);
        m_webSocketWorkers[connectionId]->MessageReceivedEvent().Subscribe(
          std::bind(&WebSocketServer::OnClientMessageReceived, this, std::placeholders::_1));

        auto args = ConnectionEventArgs(connectionId, m_webSocketWorkers[connectionId]->Ipv4Address());
        m_openConnectionEvent.Notify(args);
      },
      .message = [this](TWebSocket* webSocket_, std::string_view message_, uWS::OpCode opCode_)
      {
        m_webSocketWorkers[webSocket_->getUserData()->connectionId]->Notify(message_);
      },
      .drain = [](TWebSocket* webSocket_)
      {
        LOG4CPLUS_WARN(log4cplus::Logger::getRoot(), "Drain connection id: " << webSocket_->getUserData()->connectionId);
      },
      .close = [this](TWebSocket* webSocket_, int code, std::string_view message_)
      {
        uint64_t connectionId = webSocket_->getUserData()->connectionId;
        LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), "Close connection id: " << connectionId);

        auto args = ConnectionEventArgs(connectionId, m_webSocketWorkers[connectionId]->Ipv4Address());
        m_closeConnectionEvent.Notify(args);

        m_webSocketWorkers.erase(connectionId);
      }})
      .listen("", portNumber_, [this](auto* token_) {
        m_wsLoop = uWS::Loop::get();
      }).run();
  }).detach();
}

void WebSocketServer::OnClientMessageReceived(MessageEventArgs& messageEventArgs_)
{
  m_messageReceivedEvent.Notify(messageEventArgs_);
}

void WebSocketServer::SendText(uint64_t connectionId_, const std::string& payload_)
{
  m_wsLoop->defer([this, connectionId_, payload_]()
  {
    auto socket = m_webSocketWorkers[connectionId_]->Socket();
    socket->cork([this, socket , &connectionId_, &payload_]()
    {
      LOG4CPLUS_TRACE(log4cplus::Logger::getRoot(), "Send reply: " << connectionId_);
      auto status = socket->send(payload_, uWS::TEXT, true);

      if (status != TWebSocket::SendStatus::SUCCESS) {
        LOG4CPLUS_ERROR(log4cplus::Logger::getRoot(), "ERROR sending text reply: " << connectionId_);
      }
    });
  });
}

void WebSocketServer::SendBinary(uint64_t connectionId_, std::string_view payload_)
{
  m_wsLoop->defer([this, connectionId_, payload_]()
  {
    auto socket = m_webSocketWorkers[connectionId_]->Socket();
    socket->cork([this, socket, &connectionId_, &payload_]()
    {
      LOG4CPLUS_TRACE(log4cplus::Logger::getRoot(), "Send reply: " << connectionId_);
      auto status = socket->send(payload_, uWS::BINARY, true);

      if (status != TWebSocket::SendStatus::SUCCESS) {
        LOG4CPLUS_ERROR(log4cplus::Logger::getRoot(), "ERROR sending binary reply: " << connectionId_);
      }
    });
  });
}
