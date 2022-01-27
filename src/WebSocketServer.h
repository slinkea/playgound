#pragma once
#include <string>

#pragma warning( push )
#pragma warning( disable : 4267 )
#include <uWebSockets/App.h>
#pragma warning(pop)

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

namespace rj = rapidjson;


class WebSocketServer
{
public:
	WebSocketServer();
	virtual ~WebSocketServer();

  struct PerSocketData {
    uint64_t Id{};
  };

  struct us_listen_socket_t* listen_socket{};
  using TWebSocket = uWS::WebSocket<false, true, PerSocketData>;

  void Run(size_t portNumber_);

private:
  rj::Document m_document;
  uint64_t m_connectionCounter{};
};
