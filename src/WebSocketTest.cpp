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
  m_server.Run(1335);
  m_server.MessageReceivedEvent().Subscribe(std::bind(&WebSocketTest::OnClientMessageReceived, this, std::placeholders::_1));
  m_server.OpenConnectionEvent().Subscribe([](ConnectionEventArgs& openConnectionEventArgs_)
  {
    size_t clientId = openConnectionEventArgs_.Id();
    clientId = 0;
  });
}

void WebSocketTest::OnClientMessageReceived(const MessageEventArgs& messageEventArgs_)
{
  uint64_t clientId = messageEventArgs_.Id();
  std::string_view msg = messageEventArgs_.Message();
  LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), "message event " << clientId);

  //if (clientId == 50) {
  //  std::this_thread::sleep_for(1s); //Fake long process time.
  //}
}

uint64_t WebSocketTest::ClientContext(WebSocketClient& wsClient_)
{
  wsClient_.Connect();

  uint64_t clientId = wsClient_.RetrieveClientId();
  //clientId = wsClient_.RetrieveClientId();

  wsClient_.Disconnect();

  return clientId;
}
TEST_F(WebSocketTest, Test1)
{
  const uint64_t MAX_CLIENT(100);
  std::vector<WebSocketClient> wsClients(MAX_CLIENT);
  std::vector<std::future<uint64_t>> connections;

  for (auto& wsClient : wsClients) {
    connections.emplace_back(std::async(std::launch::async, &WebSocketTest::ClientContext, this, std::ref(wsClient)));
  }

  uint64_t checksum{};
  for (auto& connection : connections) {
    checksum += connection.get();
  }

  EXPECT_EQ(checksum, 5050);
}
