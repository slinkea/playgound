#include "WebSocketWorker.h"

#include <unordered_map>

#include <log4cplus/loggingmacros.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

using namespace std::chrono_literals;


WebSocketWorker::~WebSocketWorker()
{
  m_running = false;
  m_cv.notify_all();

  if (m_thread.joinable()) {
    m_thread.join();
  }
}

void WebSocketWorker::Initialize(uint64_t connectionId_, TWebSocket* socket_)
{
  m_webSocket = socket_;
  m_connectionId = connectionId_;

  std::thread(&WebSocketWorker::Run, this).swap(m_thread);
  while (!m_running) {
    std::this_thread::yield();
  }

  std::stringstream ipv4;
  std::string_view remoteAddress = m_webSocket->getRemoteAddress();
  if (remoteAddress.length() == 16)
  {
    ipv4 << std::to_string(static_cast<uint8_t>(remoteAddress[12])) << ".";
    ipv4 << std::to_string(static_cast<uint8_t>(remoteAddress[13])) << ".";
    ipv4 << std::to_string(static_cast<uint8_t>(remoteAddress[14])) << ".";
    ipv4 << std::to_string(static_cast<uint8_t>(remoteAddress[15]));
    m_ipv4 = ipv4.str();
  }
}

void WebSocketWorker::Notify(std::string_view message_)
{
  m_message = message_;
  m_cv.notify_all();
}

void WebSocketWorker::Run()
{
  m_running = true;
  std::mutex mutex;
  std::unique_lock<std::mutex> newMessageLock(mutex);

  try
  {
    do
    {
      m_cv.wait(newMessageLock);
      if (m_running)
      {
        LOG4CPLUS_TRACE(log4cplus::Logger::getRoot(), "Receive request from id: " << m_connectionId);
        auto args = MessageEventArgs(m_connectionId, m_message);
        m_messageReceivedEvent.Notify(args);
      }
    } while (m_running);
  }
  catch (const std::exception& ex_)
  {
    m_running = false;
    LOG4CPLUS_ERROR(log4cplus::Logger::getRoot(), "ERROR: " << ex_.what() << ", " << m_connectionId);
  }
}
