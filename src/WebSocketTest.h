#pragma once
#include <gtest/gtest.h>

#include "WebSocketServer.h"
#include "WebSocketClient.h"


class WebSocketTest : public testing::Test
{
public:
	WebSocketTest();
	~WebSocketTest();

	uint64_t MockClient(WebSocketClient& client_);
	void OnClientMessageReceived(MessageEventArgs& messageEventArgs_);

protected:
	void SetUp() override;
	void TearDown() override {}

	WebSocketServer m_server;
};
