#pragma once
#include <string>
#include <future>
#include <memory>
#include <vector>

#pragma warning( push )
#pragma warning( disable : 4267 )
#include <uWebSockets/App.h>
#pragma warning(pop)
#include <rapidjson/document.h>

#include "Event.hpp"
#include "EventArguments.h"

namespace rj = rapidjson;


struct PerSocketData {
  uint64_t connectionId{};
};

using TWebSocket = uWS::WebSocket<false, true, PerSocketData>;

class WebSocketWorker
{
public:
  WebSocketWorker() = default;
  ~WebSocketWorker();

  void Notify(std::string_view message_);
  TWebSocket* Socket() const { return m_webSocket; }
  void Initialize(uint64_t connectionId_, TWebSocket* socket_);
  const std::string& Ipv4Address() const { return m_ipv4; }

  ONDTLib::Event<WebSocketWorker, MessageEventArgs&>& MessageReceivedEvent() { return m_messageReceivedEvent; }

private:
  void Run();

  std::string m_ipv4;
  std::thread m_thread;
  rj::Document m_document;
  TWebSocket* m_webSocket{};
  uint64_t m_connectionId{};
  std::mutex m_mutexMessages;
  std::condition_variable m_cv;
  std::atomic<bool> m_running{};
  std::vector<std::string> m_messages;
  ONDTLib::Event<WebSocketWorker, MessageEventArgs&> m_messageReceivedEvent;
};
using TWebSocketWorkerPtr = std::unique_ptr<WebSocketWorker>;
