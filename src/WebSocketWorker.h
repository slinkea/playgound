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
  WebSocketWorker(uint64_t clienId_);
  virtual ~WebSocketWorker();

  void Start();
  void Stop();
  void Notify(std::string_view message_);
  void Socket(TWebSocket* webSocket_) { m_webSocket = webSocket_; }
  void Loop(uWS::Loop* wsLoop_) { m_wsLoop = wsLoop_; }
  ONDTLib::Event<WebSocketWorker, MessageEventArgs&>& MessageReceivedEvent() { return m_messageReceivedEvent; }

private:
  void Run();

  std::thread m_thread;
  std::promise<uint64_t> m_promise;
  std::condition_variable m_cv;
  std::atomic<bool> m_running{};
  TWebSocket* m_webSocket{};
  std::atomic<uint64_t> m_clienId{};
  uWS::Loop* m_wsLoop{};
  std::mutex m_mutexMessages;
  std::vector<std::string> m_messages;
  ONDTLib::Event<WebSocketWorker, MessageEventArgs&> m_messageReceivedEvent;
};
using TWebSocketWorkerPtr = std::unique_ptr<WebSocketWorker>;
