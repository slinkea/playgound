#pragma once
#include <string>


class ConnectionEventArgs
{
public:
  ConnectionEventArgs(size_t id_, const std::string& ipv4_)
    : m_id(id_),
    m_ipv4(ipv4_)
  {
  }

  size_t Id() const { return m_id; }
  const std::string& Ipv4() const { return m_ipv4; }

private:
  size_t        m_id{};
  std::string   m_ipv4;
};

//

class MessageEventArgs
{
public:
  MessageEventArgs(size_t id_, const std::string_view& request_)
    : m_id(id_)
    , m_request(request_)
  {
  }

  size_t Id() const { return m_id; }
  std::string Reply() const { return m_reply; }
  void Reply(const std::string_view reply_) { m_reply = reply_; }
  const std::string_view Request() const { return m_request; }

private:
  size_t m_id{};
  std::string m_reply;
  std::string_view m_request;
};
