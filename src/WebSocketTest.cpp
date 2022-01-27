#include "WebSocketTest.h"

#include <chrono>
#include <future>
#include "rapidjson/document.h"

namespace rj = rapidjson;


WebSocketTest::WebSocketTest()
{
}

WebSocketTest::~WebSocketTest()
{
}

void WebSocketTest::SetUp()
{
  m_server.Run(1333);
}

void WebSocketTest::TearDown()
{
}

void WebSocketTest::Run(WebSocketClient& wsClient_, std::atomic<uint64_t>& checksum_)
{
  wsClient_.Connect();
  uint64_t clientId = wsClient_.RetrieveClientId();
  clientId = wsClient_.RetrieveClientId();
  clientId = wsClient_.RetrieveClientId();
  clientId = wsClient_.RetrieveClientId();
  clientId = wsClient_.RetrieveClientId();
  checksum_ += clientId;
  wsClient_.Disconnect();
}
TEST_F(WebSocketTest, Test1)
{
  WebSocketClient wsClient;

  for (size_t counter{1}; counter <= 100; counter++)
  {
    wsClient.Connect();
    uint64_t clientId = wsClient.RetrieveClientId();
    EXPECT_EQ(clientId, counter);
    wsClient.Disconnect();
  }
}

TEST_F(WebSocketTest, Test2)
{
  WebSocketClient wsClient1;
  WebSocketClient wsClient2;

  wsClient1.Connect();
  wsClient2.Connect();

  wsClient2.Disconnect();
  wsClient1.Disconnect();
}

TEST_F(WebSocketTest, Test3)
{
  std::vector<WebSocketClient> wsClients(100);
  std::vector<std::future<void>> connections;

  std::atomic<uint64_t> checksum{};
  for (auto& wsClient : wsClients)
    connections.emplace_back(std::async(&WebSocketTest::Run, this, std::ref(wsClient), std::ref(checksum)));

  for (auto& connection : connections)
    connection.get();

  EXPECT_EQ(checksum, 5050);
}