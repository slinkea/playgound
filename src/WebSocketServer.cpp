#include "WebSocketServer.h"

#include <log4cplus/loggingmacros.h>

using namespace std::chrono_literals;


WebSocketServer::WebSocketServer()
{
  m_document.Parse("{\"jsonrpc\":\"2.0\",\"result\":{\"clientId\":0},\"id\":1}");
}

WebSocketServer::~WebSocketServer()
{
}

void WebSocketServer::Initialize()
{
  for (uint64_t clientId{1}; clientId <= MAX_CLIENT; clientId++) {
    m_webSocketWorkers.emplace(clientId, std::make_unique<WebSocketWorker>(clientId));
  }
}

void WebSocketServer::Shutdown()
{
  for (auto& webSocketWorker : m_webSocketWorkers) {
    webSocketWorker.second->Stop();
  }
}

void WebSocketServer::Run(size_t portNumber_)
{
  std::thread([this, portNumber_]() {
    uWS::App().ws<PerSocketData>("/*",
    {
      .compression = uWS::DEDICATED_COMPRESSOR_256KB, //DISABLED
      .maxPayloadLength = 16 * 1024 * 1024, //16 * 1024
      .idleTimeout = 0, //120
      .maxBackpressure = 0, //64 * 1024

    .upgrade = [this](auto* res_, auto* req_, auto* context_)
    {
      m_connectionCounter++;
      LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), "upgrade: " << m_connectionCounter);

      res_->cork([this, res_, req_, context_]()
      {
        res_->template upgrade<PerSocketData>({
          .Id = m_connectionCounter
          }, 
          req_->getHeader("sec-websocket-key"),
          req_->getHeader("sec-websocket-protocol"),
          req_->getHeader("sec-websocket-extensions"),
          context_);
      });
    },
    .open = [this](TWebSocket* webSocket_)
    {
      LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), "open");
      uint64_t clientId = webSocket_->getUserData()->Id;
      m_webSocketWorkers[clientId]->Socket(webSocket_);
      m_webSocketWorkers[clientId]->Loop(m_wsLoop);
      m_webSocketWorkers[clientId]->MessageReceivedEvent().Subscribe(
        std::bind(&WebSocketServer::OnClientMessageReceived, this, std::placeholders::_1));

      std::stringstream ipv4;
      std::string_view remoteAddress = webSocket_->getRemoteAddress();
      if (remoteAddress.length() == 16)
      {
        ipv4 << std::to_string(static_cast<uint8_t>(remoteAddress[12])) << ".";
        ipv4 << std::to_string(static_cast<uint8_t>(remoteAddress[13])) << ".";
        ipv4 << std::to_string(static_cast<uint8_t>(remoteAddress[14])) << ".";
        ipv4 << std::to_string(static_cast<uint8_t>(remoteAddress[15]));
      }

      auto args = ConnectionEventArgs(clientId, ipv4.str());
      m_openConnectionEvent.Notify(args);
    },
    .message = [this](TWebSocket* webSocket_, std::string_view message_, uWS::OpCode opCode_)
    {
      LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), "message");
      m_webSocketWorkers[webSocket_->getUserData()->Id]->Notify(message_);
    },
    .drain = [](TWebSocket* webSocket_)
    {
       LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), "drain");
    },
    .close = [this](TWebSocket* webSocket_, int code, std::string_view message_)
    {
      LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), "close");
      m_webSocketWorkers[webSocket_->getUserData()->Id]->Stop();

    }})
    .listen("", (int)portNumber_, [this, portNumber_](auto* token) {      
      m_wsLoop = uWS::Loop::get();
      listen_socket = token;

    }).run();
  }).detach();
}

void WebSocketServer::OnClientMessageReceived(MessageEventArgs& messageEventArgs_)
{
  m_messageReceivedEvent.Notify(messageEventArgs_);
}
