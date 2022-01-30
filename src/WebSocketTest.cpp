#include "WebSocketTest.h"

#include <iostream>
#include <chrono>
#include <future>
#include <filesystem>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <log4cplus/log4cplus.h>

namespace rj = rapidjson;
namespace fs = std::filesystem;
using namespace std::chrono_literals;


WebSocketTest::WebSocketTest()
{
  log4cplus::initialize();
  fs::path pathProps(fs::current_path() / LR"(log.properties)");
  log4cplus::PropertyConfigurator config(pathProps.wstring());
  config.configure();
}

WebSocketTest::~WebSocketTest()
{
  log4cplus::Logger::shutdown();
}

void WebSocketTest::SetUp()
{
  m_server.Run(1335);
  m_server.MessageReceivedEvent().Subscribe(std::bind(&WebSocketTest::OnClientMessageReceived, this, std::placeholders::_1));

  m_server.OpenConnectionEvent().Subscribe([](ConnectionEventArgs& openConnectionEventArgs_) {
    uint64_t connectionId = openConnectionEventArgs_.Id();
  });

  m_server.CloseConnectionEvent().Subscribe([](ConnectionEventArgs& openConnectionEventArgs_) {
    uint64_t connectionId = openConnectionEventArgs_.Id();
  });
}

uint64_t WebSocketTest::MokeClient(WebSocketClient& client_)
{
  client_.Connect();
  uint64_t connectionId = client_.RetrieveConnectionId();
  connectionId = client_.RetrieveConnectionId();
  connectionId = client_.RetrieveConnectionId();
  connectionId = client_.RetrieveConnectionId();
  connectionId = client_.RetrieveConnectionId();
  client_.Disconnect();

  //client_.Connect();
  //connectionId += client_.RetrieveConnectionId();  
  //client_.Disconnect();

  return connectionId;
}

void WebSocketTest::OnClientMessageReceived(MessageEventArgs& messageEventArgs_)
{
  uint64_t connectionId = messageEventArgs_.Id();
  std::string_view request = messageEventArgs_.Message();

  std::this_thread::sleep_for(1s);

  //if (connectionId == 1) {
  //  std::this_thread::sleep_for(2s);
  //}
  //else if (connectionId == 2) {
  //  std::this_thread::sleep_for(4s);
  //}
  //else if (connectionId == 3) {
  //  throw std::exception("KABOOM");
  //}
  //else if (connectionId == 4) {
  //  std::this_thread::sleep_for(2s);
  //}
  //else if (connectionId == 5) {
  //  std::this_thread::sleep_for(1s);
  //}

  rj::StringBuffer buffer;
  rj::Writer<rj::StringBuffer> writer(buffer);
  rj::Document document;
  document.Parse("{\"jsonrpc\":\"2.0\",\"result\":{\"clientId\":0},\"id\":1}");
  document["result"]["clientId"] = connectionId;
  document.Accept(writer);

  m_server.SendText(connectionId, buffer.GetString());
}
TEST_F(WebSocketTest, Test1)
{
  const uint64_t MAX_CLIENT(1);
  std::vector<std::future<uint64_t>> mokeClients;
  std::vector<WebSocketClient> clients(MAX_CLIENT);

  for (auto& client : clients) {
    mokeClients.emplace_back(std::async(std::launch::async, &WebSocketTest::MokeClient, this, std::ref(client)));
  }

  uint64_t checksum{};
  for (auto& mokeClient : mokeClients) {
    checksum += mokeClient.get();
  }

  EXPECT_EQ(checksum, 1); //100 = 5050
}
