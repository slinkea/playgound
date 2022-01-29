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

namespace rj = rapidjson;


class WebSocketServer
{
public:
	WebSocketServer();
	~WebSocketServer() = default;

  void Run(int portNumber_);

  ONDTLib::Event<WebSocketServer, ConnectionEventArgs&>& OpenConnectionEvent() { return m_openConnectionEvent; }
  ONDTLib::Event<WebSocketServer, MessageEventArgs&>& MessageReceivedEvent() { return m_messageReceivedEvent; }

  void OnClientMessageReceived(MessageEventArgs& messageEventArgs_);

private:
  rj::Document m_document;
  std::atomic<uint64_t> m_connectionCounter{};
  std::unordered_map<uint64_t, TWebSocketWorkerPtr> m_webSocketWorkers;
  ONDTLib::Event<WebSocketServer, ConnectionEventArgs&> m_openConnectionEvent;
  ONDTLib::Event<WebSocketServer, MessageEventArgs&> m_messageReceivedEvent;
};
