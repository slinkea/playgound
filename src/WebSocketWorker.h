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
  WebSocketWorker() = delete;
  WebSocketWorker(uint64_t clientId_);
  ~WebSocketWorker();

  void Notify(std::string_view message_);
  bool IsRunning() const { return m_running; }
  TWebSocket* Socket() const { return m_webSocket; }
  void Socket(TWebSocket* socket_) { m_webSocket = socket_; }
  
  ONDTLib::Event<WebSocketWorker, MessageEventArgs&>& MessageReceivedEvent() { return m_messageReceivedEvent; }

private:
  void Run();

  uint64_t m_clienId{};
  std::thread m_thread;
  TWebSocket* m_webSocket{};
  std::mutex m_mutexMessages;
  std::condition_variable m_cv;
  std::atomic<bool> m_running{};
  std::vector<std::string> m_messages;
  ONDTLib::Event<WebSocketWorker, MessageEventArgs&> m_messageReceivedEvent;
};
using TWebSocketWorkerPtr = std::unique_ptr<WebSocketWorker>;
