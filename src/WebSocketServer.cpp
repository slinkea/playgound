#include "WebSocketServer.h"


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
      auto userData = webSocket_->getUserData();
    },
    .message = [this](TWebSocket* webSocket_, std::string_view message_, uWS::OpCode opCode_)
    {
      rj::StringBuffer buffer;
      rj::Writer<rj::StringBuffer> writer(buffer);
      m_document["result"]["clientId"] = webSocket_->getUserData()->Id;
      m_document.Accept(writer);

      webSocket_->cork([&webSocket_, &buffer]()
      {
        TWebSocket::SendStatus status = webSocket_->send(buffer.GetString(), uWS::TEXT, true);
        if (status != TWebSocket::SendStatus::SUCCESS) {
          status = TWebSocket::SendStatus::SUCCESS;
        }
      });
    },
    .drain = [](TWebSocket* webSocket_)
    {
      auto userData = webSocket_->getUserData();
    },
    .ping = [](TWebSocket* pWebSocket_, std::string_view message_)
    {
      auto userData = pWebSocket_->getUserData();
    },
    .close = [](TWebSocket* webSocket_, int code, std::string_view message_)
    {
      auto userData = webSocket_->getUserData();

    }})
    .listen("", (int)portNumber_, [this, portNumber_](auto* token) { listen_socket = token; }).run();
  }).detach();
}

