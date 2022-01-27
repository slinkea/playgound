#include "WebSocketTest.h"

#include <chrono>
#include <future>
#include <filesystem>
#include <rapidjson/document.h>
#include <log4cplus/log4cplus.h>

namespace rj = rapidjson;
namespace fs = std::filesystem;


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
  m_server.Run(1333);
}

void WebSocketTest::TearDown()
{
}

void WebSocketTest::Run(WebSocketClient& wsClient_, std::atomic<uint64_t>& checksum_)
{
  LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), "Run");

  wsClient_.Connect();
  uint64_t clientId = wsClient_.RetrieveClientId();
  checksum_ += clientId;
  wsClient_.Disconnect();
}
TEST_F(WebSocketTest, Test1)
{
  WebSocketClient wsClient;

  for (size_t counter{1}; counter <= 2; counter++)
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
  std::vector<WebSocketClient> wsClients(1000);
  std::vector<std::future<void>> connections;

  std::atomic<uint64_t> checksum{};
  for (auto& wsClient : wsClients)
    connections.emplace_back(std::async(std::launch::async, &WebSocketTest::Run, this, std::ref(wsClient), std::ref(checksum)));

  for (auto& connection : connections)
    connection.get();

  EXPECT_EQ(checksum, 500500);
}

TEST_F(WebSocketTest, Test4)
{
  std::vector<WebSocketClient> wsClients(2);
  std::vector<std::future<void>> connections;

  std::atomic<uint64_t> checksum{};
  for (auto& wsClient : wsClients)
    connections.emplace_back(std::async(std::launch::async, &WebSocketTest::Run, this, std::ref(wsClient), std::ref(checksum)));

  for (auto& connection : connections)
    connection.get();

  EXPECT_EQ(checksum, 500500);
}