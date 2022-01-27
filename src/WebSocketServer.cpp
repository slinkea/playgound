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
    .open = [](TWebSocket* webSocket_)
    {
      LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), "open: " << webSocket_->getUserData()->Id);
    },
    .message = [this](TWebSocket* webSocket_, std::string_view message_, uWS::OpCode opCode_)
    {
      LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), "message: " << webSocket_->getUserData()->Id);

      webSocket_->cork([&webSocket_]()
      {
        LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), "message cork: " << webSocket_->getUserData()->Id);
        std::this_thread::sleep_for(1s);

        rj::StringBuffer buffer;
        rj::Writer<rj::StringBuffer> writer(buffer);
        rj::Document document;
        document.Parse("{\"jsonrpc\":\"2.0\",\"result\":{\"clientId\":0},\"id\":1}");
        document["result"]["clientId"] = webSocket_->getUserData()->Id;
        document.Accept(writer);

        LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), "message send: " << webSocket_->getUserData()->Id);
        auto status = webSocket_->send(buffer.GetString(), uWS::TEXT, true);
        if (status != TWebSocket::SendStatus::SUCCESS) {
          LOG4CPLUS_ERROR(log4cplus::Logger::getRoot(), "ERROR message send: " << webSocket_->getUserData()->Id);
        }
      });
    },
    .drain = [](TWebSocket* webSocket_)
    {
      LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), "drain: " << webSocket_->getUserData()->Id);
    },
    .close = [](TWebSocket* webSocket_, int code, std::string_view message_)
    {
      LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), "close: " << webSocket_->getUserData()->Id);

    }})
    .listen("", (int)portNumber_, [this, portNumber_](auto* token) { listen_socket = token; }).run();
  }).detach();
}

