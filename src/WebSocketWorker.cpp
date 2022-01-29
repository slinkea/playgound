#include "WebSocketWorker.h"

#include <log4cplus/loggingmacros.h>

using namespace std::chrono_literals;


WebSocketWorker::WebSocketWorker()
{
  std::thread(&WebSocketWorker::Run, this).swap(m_thread);
}

WebSocketWorker::~WebSocketWorker()
{
  m_running = false;
  m_cv.notify_all();

  if (m_thread.joinable()) {
    m_thread.join();
  }
}

void WebSocketWorker::Notify(std::string_view message_)
{
  m_mutexMessages.lock();
  m_messages.push_back(message_.data());
  m_mutexMessages.unlock();

  m_cv.notify_all();
}

void WebSocketWorker::Initialize(TWebSocket* webSocket_, uWS::Loop* wsLoop_, uint64_t clientId_)
{
  m_wsLoop = wsLoop_;
  m_clienId = clientId_;
  m_webSocket = webSocket_;
}

void WebSocketWorker::Run()
{
  m_running = true;
  std::mutex mutex;
  std::string message;
  std::unique_lock<std::mutex> newMessageLock(mutex);

  try
  {
    do
    {
      m_cv.wait(newMessageLock);
      if (m_running)
      {
        m_mutexMessages.lock();
        if (m_messages.size() > 0)
        {
          message = m_messages.front();
          m_messages.pop_back();
        }
        m_mutexMessages.unlock();

        auto args = MessageEventArgs(m_clienId, message);
        LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), "Request received " << m_clienId);
        m_messageReceivedEvent.Notify(args);
        std::string reply(args.Reply());
        message.clear();

        m_wsLoop->defer([this, reply]()
        {
          m_webSocket->cork([this, &reply]()
          {
            LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), "Send reply: " << m_clienId);
            auto status = m_webSocket->send(reply, uWS::TEXT, true);
            if (status != TWebSocket::SendStatus::SUCCESS) {
              LOG4CPLUS_ERROR(log4cplus::Logger::getRoot(), "ERROR sending reply: " << m_clienId);
            }
          });
        });
      }
    } while (m_running);
  }
  catch (const std::exception& ex_)
  {
    m_running = false;
    LOG4CPLUS_ERROR(log4cplus::Logger::getRoot(), "ERROR: " << ex_.what() << ", " << m_clienId);
  }
}
