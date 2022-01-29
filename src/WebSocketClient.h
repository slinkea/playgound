#pragma once
#include <future>

#pragma warning( push )
#pragma warning( disable : 4267 )
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/connection.hpp>
#pragma warning(pop)


class WebSocketClient
{
public:
	WebSocketClient();
	~WebSocketClient();

	void Connect();
	void Disconnect();
	uint64_t RetrieveClientId();

private:
	using TClientMessage = websocketpp::config::asio_client::message_type::ptr;
	using TAsioClient = websocketpp::client<websocketpp::config::asio_client>;

	TAsioClient m_client;
	websocketpp::connection_hdl m_hdl;
	websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_thread;

	void OnOpen(websocketpp::connection_hdl hdl_);
	void OnMessage(websocketpp::connection_hdl hdl_, const TClientMessage& message_);
	void OnClose(websocketpp::connection_hdl hdl_);
	void OnFail(websocketpp::connection_hdl hdl_);

	std::mutex m_mtx;
	std::string m_message;
	std::condition_variable m_cv;
};
