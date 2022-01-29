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
  MessageEventArgs(size_t id_, const std::string_view& message_)
    : m_id(id_)
    , m_message(message_)
  {
  }

  size_t Id() const { return m_id; }
  const std::string_view Message() const { return m_message; }

private:
  size_t m_id{};
  std::string_view m_message;
};
