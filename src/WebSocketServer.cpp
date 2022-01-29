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

WebSocketServer::~WebSocketServer()
{
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
        m_webSocketWorkers.emplace(++m_connectionCounter, std::make_unique<WebSocketWorker>());
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
        m_webSocketWorkers.erase(webSocket_->getUserData()->Id);
      }
      }).listen("", (int)portNumber_, [this, portNumber_](auto* token) {
        m_wsLoop = uWS::Loop::get();
        listen_socket = token;
      }).run();
  }).detach();
}

void WebSocketServer::RunParallel(size_t portNumber_)
{
  m_threads.resize(64 /*std::thread::hardware_concurrency()*/);

  std::transform(m_threads.begin(), m_threads.end(), m_threads.begin(), [this](std::thread*/*t*/) {
    return new std::thread([this]() {
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
        LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), "open: " << webSocket_->getUserData()->Id);
      },
      .message = [this](TWebSocket* webSocket_, std::string_view message_, uWS::OpCode opCode_)
      {
        LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), "message: " << webSocket_->getUserData()->Id);

        if (webSocket_->getUserData()->Id == 4) {
         std::this_thread::sleep_for(2s);
        }

        //std::this_thread::sleep_for(3s); //Fake long process time.

        //webSocket_->cork([webSocket_]()
        //{
          rj::StringBuffer buffer;
          rj::Writer<rj::StringBuffer> writer(buffer);
          rj::Document document;
          document.Parse("{\"jsonrpc\":\"2.0\",\"result\":{\"clientId\":0},\"id\":1}");
          document["result"]["clientId"] = webSocket_->getUserData()->Id;
          document.Accept(writer);

          auto status = webSocket_->send(buffer.GetString(), uWS::TEXT, true);
          if (status != TWebSocket::SendStatus::SUCCESS) {
            LOG4CPLUS_ERROR(log4cplus::Logger::getRoot(), "ERROR message send: " << webSocket_->getUserData()->Id);
          }
        //});
      },
      .drain = [](TWebSocket* webSocket_)
      {
        LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), "drain: " << webSocket_->getUserData()->Id);
      },
      .close = [this](TWebSocket* webSocket_, int code, std::string_view message_)
      {
        LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), "close: " << webSocket_->getUserData()->Id);
      }})
      .listen("", 1335, [](auto* listen_socket) {
        if (listen_socket) {
          LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), "listening on port 1335, Loop " << uWS::Loop::get());
        }
        else {
          LOG4CPLUS_ERROR(log4cplus::Logger::getRoot(), "failed to listen on port 1335");
        }
      }).run();
    });
  });
}

void WebSocketServer::OnClientMessageReceived(MessageEventArgs& messageEventArgs_)
{
  m_messageReceivedEvent.Notify(messageEventArgs_);
}
