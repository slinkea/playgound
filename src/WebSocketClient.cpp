#include "WebSocketClient.h"

#pragma warning( push )
#pragma warning( disable : 4996 )
#include <asio/impl/src.hpp>
#pragma warning(pop)

#include <chrono>
#include "rapidjson/document.h"

namespace rj = rapidjson;
using namespace std::chrono_literals;


WebSocketClient::WebSocketClient()
{
  m_client.clear_access_channels(websocketpp::log::alevel::all);
  m_client.clear_error_channels(websocketpp::log::alevel::all);

  m_client.init_asio();
  m_client.start_perpetual();
  m_client.set_open_handshake_timeout(60000);
  m_thread.reset(new websocketpp::lib::thread(&TAsioClient::run, &m_client));
}

WebSocketClient::~WebSocketClient()
{
  m_client.stop_perpetual();
  if (m_thread->joinable()) {
    m_thread->join();
  }
}

void WebSocketClient::Connect()
{
  websocketpp::lib::error_code ec;
  TAsioClient::connection_ptr connection = m_client.get_connection("ws://127.0.0.1:1335", ec);

  if (!ec)
  {
    m_hdl = connection->get_handle();
    connection->set_open_handler(std::bind(&WebSocketClient::OnOpen, this, std::placeholders::_1));
    connection->set_message_handler(std::bind(&WebSocketClient::OnMessage, this, std::placeholders::_1, std::placeholders::_2));
    connection->set_close_handler(std::bind(&WebSocketClient::OnClose, this, std::placeholders::_1));
    connection->set_fail_handler(std::bind(&WebSocketClient::OnFail, this, std::placeholders::_1));
    m_client.connect(connection);

    std::unique_lock<std::mutex> lk(m_mtx);
    m_cv.wait_for(lk, 5s, [&] {
      return connection->get_state() == websocketpp::session::state::value::open; 
    });
  }
}

void WebSocketClient::Disconnect()
{
  TAsioClient::client::connection_ptr connection = m_client.get_con_from_hdl(m_hdl);
  if (connection->get_state() == websocketpp::session::state::value::open)
  {
    websocketpp::lib::error_code ec;
    m_client.close(m_hdl, websocketpp::close::status::going_away, "", ec);

    std::unique_lock<std::mutex> lk(m_mtx);
    m_cv.wait_for(lk, 5s, [&] {
      return connection->get_state() == websocketpp::session::state::value::closed;
    });
  }
}

uint64_t WebSocketClient::RetrieveConnectionId()
{
  m_message.clear();
  std::string message("{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"getClientId\"}");

  rj::Document document;
  if (!document.Parse(message.c_str()).HasParseError())
  {
    if (document["method"].GetString() == "getClientId")
    {
      bool x{};
      x = true;
    }
  }

  m_client.send(m_hdl, message, websocketpp::frame::opcode::text);

  std::unique_lock<std::mutex> lk(m_mtx);
  m_cv.wait_for(lk, 7s, [&] { return !m_message.empty(); });

  //rj::Document document;
  if (!document.Parse(m_message.c_str()).HasParseError()) {
    return document["result"]["clientId"].GetUint64();
  }

  return 0;
}

void WebSocketClient::OnOpen(websocketpp::connection_hdl hdl_)
{
  m_cv.notify_all();
}

void WebSocketClient::OnMessage(websocketpp::connection_hdl hdl_, const TClientMessage& message_)
{
  m_message = message_->get_payload();
  m_cv.notify_all();
}

void WebSocketClient::OnClose(websocketpp::connection_hdl hdl_)
{
  m_cv.notify_all();
}

void WebSocketClient::OnFail(websocketpp::connection_hdl hdl_)
{
  TAsioClient::client::connection_ptr connection = m_client.get_con_from_hdl(hdl_);
  std::string errorMessage = connection->get_ec().message();
}
