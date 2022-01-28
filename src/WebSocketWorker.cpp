#include "WebSocketWorker.h"

#include <log4cplus/loggingmacros.h>
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

namespace rj = rapidjson;

using namespace std::chrono_literals;


WebSocketWorker::WebSocketWorker(uint64_t clienId_)
  : m_clienId(clienId_)
{
  Start();
}

WebSocketWorker::~WebSocketWorker()
{
  m_running = false;
  if (m_thread.joinable()) {
    m_thread.join();
  }
}

void WebSocketWorker::Start()
{
  m_running = true;
  m_promise = std::promise<uint64_t>();
  std::thread(&WebSocketWorker::Run, this).swap(m_thread);
}

void WebSocketWorker::Stop()
{
  m_running = false;
  m_cv.notify_all();

  if (m_thread.joinable())
  {
    m_thread.join();
    m_promise.get_future().get();
  }
}

void WebSocketWorker::Notify(std::string_view message_)
{
  m_mutexMessages.lock();
  m_messages.push_back(message_.data());
  m_mutexMessages.unlock();

  m_cv.notify_all();
}

void WebSocketWorker::Run()
{
  LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), "WorkerRun Begin: " << m_clienId.load());

  std::string message;
  bool exceptionFound{};
  std::mutex mutex;
  std::unique_lock<std::mutex> newMessageLock(mutex);

  try
  {
    do
    {
      m_cv.wait(newMessageLock);

      m_mutexMessages.lock();
      if (m_messages.size() > 0)
      {
        message = m_messages.front();
        m_messages.pop_back();
      }
      m_mutexMessages.unlock();

      if (!message.empty())
      {
        auto args = MessageEventArgs(m_clienId, message);
        m_messageReceivedEvent.Notify(args);
        message.clear();

        m_wsLoop->defer([this]()
        {
          rj::StringBuffer buffer;
          rj::Writer<rj::StringBuffer> writer(buffer);
          rj::Document document;
          document.Parse("{\"jsonrpc\":\"2.0\",\"result\":{\"clientId\":0},\"id\":1}");
          document["result"]["clientId"] = m_clienId.load();
          document.Accept(writer);

          LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), "send: " << m_clienId);
          m_webSocket->cork([this, &buffer]()
          {
            auto status = m_webSocket->send(buffer.GetString(), uWS::TEXT, true);
            if (status != TWebSocket::SendStatus::SUCCESS) {
              LOG4CPLUS_ERROR(log4cplus::Logger::getRoot(), "ERROR message send: " << m_clienId);
            }
          });
        });
      }

    } while (m_running);
  }
  catch (const std::exception&)
  {
    exceptionFound = true;
    m_promise.set_exception(std::current_exception());
  }

  if (!exceptionFound) {
    m_promise.set_value(m_clienId);
  }

  LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), "WorkerRun Done: " << m_clienId);

  m_running = false;
  m_cv.notify_all();
}
