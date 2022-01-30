#pragma once
#include <unordered_map>

#pragma warning( push )
#pragma warning( disable : 4267 )
#include <uWebSockets/App.h>
#pragma warning(pop)

#include "Event.hpp"
#include "EventArguments.h"
#include "WebSocketWorker.h"


class WebSocketServer
{
public:
	WebSocketServer() = default;
	~WebSocketServer() = default;

  void Run(int portNumber_);
  void SendText(uint64_t connectionId_, const std::string& payload_);
  void SendBinary(uint64_t connectionId_, std::string_view payload_);

  ONDTLib::Event<WebSocketServer, MessageEventArgs&>& MessageReceivedEvent() { return m_messageReceivedEvent; }
  ONDTLib::Event<WebSocketServer, ConnectionEventArgs&>& OpenConnectionEvent() { return m_openConnectionEvent; }
  ONDTLib::Event<WebSocketServer, ConnectionEventArgs&>& CloseConnectionEvent() { return m_closeConnectionEvent; }

  void OnClientMessageReceived(MessageEventArgs& messageEventArgs_);

private:
  uWS::Loop* m_wsLoop{};
  std::atomic<uint64_t> m_connectionCounter{};
  std::unordered_map<uint64_t, TWebSocketWorkerPtr> m_webSocketWorkers;
  ONDTLib::Event<WebSocketServer, MessageEventArgs&> m_messageReceivedEvent;
  ONDTLib::Event<WebSocketServer, ConnectionEventArgs&> m_openConnectionEvent;
  ONDTLib::Event<WebSocketServer, ConnectionEventArgs&> m_closeConnectionEvent;
};
