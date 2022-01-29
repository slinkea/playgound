#include "WebSocketServer.h"

#include <algorithm>
#include <log4cplus/loggingmacros.h>
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

namespace rj = rapidjson;
using namespace std::chrono_literals;


WebSocketServer::WebSocketServer()
{
  m_document.Parse("{\"jsonrpc\":\"2.0\",\"result\":{\"clientId\":0},\"id\":1}");
}

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
      LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), "upgrade: " << ++m_connectionCounter);
        m_webSocketWorkers.emplace(m_connectionCounter, std::make_unique<WebSocketWorker>());

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
        uint64_t clientId = webSocket_->getUserData()->Id;
        LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), "open " << clientId);

        while (!m_webSocketWorkers[clientId]->IsRunning()) {
          std::this_thread::yield();
        }

        m_webSocketWorkers[clientId]->Initialize(webSocket_, uWS::Loop::get(), clientId);
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

          auto args = ConnectionEventArgs(clientId, ipv4.str());
          m_openConnectionEvent.Notify(args);
        }
      },
      .message = [this](TWebSocket* webSocket_, std::string_view message_, uWS::OpCode opCode_)
      {
        uint64_t clientId = webSocket_->getUserData()->Id;
        LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), "message " << clientId);
        m_webSocketWorkers[clientId]->Notify(message_);
      },
      .drain = [](TWebSocket* webSocket_)
      {
        LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), "drain " << webSocket_->getUserData()->Id);
      },
      .close = [this](TWebSocket* webSocket_, int code, std::string_view message_)
      {
        uint64_t clientId = webSocket_->getUserData()->Id;
        LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), "close " << clientId);
        m_webSocketWorkers.erase(clientId);
      }})
      .listen("", portNumber_, [](auto* token_) {
      }).run();
  }).detach();
}

void WebSocketServer::OnClientMessageReceived(MessageEventArgs& messageEventArgs_)
{
  m_messageReceivedEvent.Notify(messageEventArgs_);
}
