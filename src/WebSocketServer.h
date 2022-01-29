#pragma once
#include <unordered_map>

#pragma warning( push )
#pragma warning( disable : 4267 )
#include <uWebSockets/App.h>
#pragma warning(pop)
#include "rapidjson/document.h"

#include "Event.hpp"
#include "EventArguments.h"
#include "WebSocketWorker.h"
#include "ThreadPool2.h"

namespace rj = rapidjson;

constexpr uint64_t MAX_CLIENT(3);


class WebSocketServer
{
public:
	WebSocketServer();
	virtual ~WebSocketServer();

  void Run(size_t portNumber_);
  void RunParallel(size_t portNumber_);

  ONDTLib::Event<WebSocketServer, ConnectionEventArgs&>& OpenConnectionEvent() { return m_openConnectionEvent; }
  ONDTLib::Event<WebSocketServer, MessageEventArgs&>& MessageReceivedEvent() { return m_messageReceivedEvent; }

  void OnClientMessageReceived(MessageEventArgs& messageEventArgs_);

private:
  struct us_listen_socket_t* listen_socket{};

  uWS::Loop* m_wsLoop{};
  rj::Document m_document;
  std::vector<std::thread*> m_threads;
  std::atomic<uint64_t> m_connectionCounter{};
  std::unordered_map<uint64_t, TWebSocketWorkerPtr> m_webSocketWorkers;
  ONDTLib::Event<WebSocketServer, ConnectionEventArgs&> m_openConnectionEvent;
  ONDTLib::Event<WebSocketServer, MessageEventArgs&> m_messageReceivedEvent;
};
