#include "WebSocketTest.h"

#include <iostream>
#include <chrono>
#include <future>
#include <filesystem>
#include <rapidjson/document.h>
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
  //m_server.RunParallel(1335);

  m_server.Run(1335);

  m_server.MessageReceivedEvent().Subscribe(std::bind(&WebSocketTest::OnClientMessageReceived, this, std::placeholders::_1));
  m_server.OpenConnectionEvent().Subscribe([](ConnectionEventArgs& openConnectionEventArgs_)
  {
    size_t clientId = openConnectionEventArgs_.Id();
    clientId = 0;
  });
}

void WebSocketTest::TearDown()
{
}

void WebSocketTest::OnClientMessageReceived(const MessageEventArgs& messageEventArgs_)
{
  uint64_t clientId = messageEventArgs_.Id();
  std::string_view msg = messageEventArgs_.Message();
  LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), "message from " << clientId);

  std::this_thread::sleep_for(2s); //Fake long process time.
}

uint64_t WebSocketTest::ClientContext(WebSocketClient& wsClient_)
{
  wsClient_.Connect();

  uint64_t clientId = wsClient_.RetrieveClientId();
  clientId = wsClient_.RetrieveClientId();

  wsClient_.Disconnect();

  return clientId;
}
TEST_F(WebSocketTest, Test1)
{
  std::vector<WebSocketClient> wsClients(MAX_CLIENT);
  std::vector<std::future<uint64_t>> connections;

  for (auto& wsClient : wsClients) {
    connections.emplace_back(std::async(std::launch::async, &WebSocketTest::ClientContext, this, std::ref(wsClient)));
  }

  uint64_t checksum{};
  for (auto& connection : connections) {
    checksum += connection.get();
  }

  EXPECT_EQ(checksum, 1);
}

TEST_F(WebSocketTest, DISABLED_Test2)
{
  std::vector<WebSocketClient> wsClients(MAX_CLIENT);
  std::vector<std::future<uint64_t>> connections;

  for (auto& wsClient : wsClients) {
    connections.emplace_back(std::async(std::launch::async, &WebSocketTest::ClientContext, this, std::ref(wsClient)));
  }

  uint64_t checksum{};
  for (auto& connection : connections) {
    checksum += connection.get();
  }
}
//TEST_F(WebSocketTest, DISABLED_Test1)
//{
//  WebSocketClient wsClient;
//
//  for (size_t counter{1}; counter <= 2; counter++)
//  {
//    wsClient.Connect();
//    uint64_t clientId = wsClient.RetrieveClientId();
//    EXPECT_EQ(clientId, counter);
//    wsClient.Disconnect();
//  }
//}
//
//TEST_F(WebSocketTest, DISABLED_Test2)
//{
//  WebSocketClient wsClient1;
//  WebSocketClient wsClient2;
//
//  wsClient1.Connect();
//  wsClient2.Connect();
//
//  wsClient2.Disconnect();
//  wsClient1.Disconnect();
//}
//
//TEST_F(WebSocketTest, DISABLED_Test3)
//{
//  std::vector<WebSocketClient> wsClients(1000);
//  std::vector<std::future<void>> connections;
//
//  std::atomic<uint64_t> checksum{};
//  for (auto& wsClient : wsClients)
//    connections.emplace_back(std::async(std::launch::async, &WebSocketTest::Run, this, std::ref(wsClient), std::ref(checksum)));
//
//  for (auto& connection : connections)
//    connection.get();
//
//  EXPECT_EQ(checksum, 500500);
//}
