#pragma once
#include <string>
#include <future>
#include <memory>
#include <vector>

#pragma warning( push )
#pragma warning( disable : 4267 )
#include <uWebSockets/App.h>
#pragma warning(pop)

#include "Event.hpp"
#include "EventArguments.h"


struct PerSocketData {
  uint64_t Id{};
};

using TWebSocket = uWS::WebSocket<false, true, PerSocketData>;

class WebSocketWorker
{
public:
  WebSocketWorker();
  ~WebSocketWorker();

  bool IsRunning() const { return m_running; }
  void Notify(std::string_view message_);
  void Initialize(TWebSocket* webSocket_, uWS::Loop* wsLoop_, uint64_t clientId_);
  ONDTLib::Event<WebSocketWorker, MessageEventArgs&>& MessageReceivedEvent() { return m_messageReceivedEvent; }

private:
  void Run();

  uint64_t m_clienId{};
  std::thread m_thread;
  uWS::Loop* m_wsLoop{};
  TWebSocket* m_webSocket{};
  std::mutex m_mutexMessages;
  std::condition_variable m_cv;
  std::atomic<bool> m_running{};
  std::vector<std::string> m_messages;
  ONDTLib::Event<WebSocketWorker, MessageEventArgs&> m_messageReceivedEvent;
};
using TWebSocketWorkerPtr = std::unique_ptr<WebSocketWorker>;
