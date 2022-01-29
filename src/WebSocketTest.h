#pragma once
#include <gtest/gtest.h>

#include "WebSocketServer.h"
#include "WebSocketClient.h"


class WebSocketTest : public testing::Test
{
public:
	WebSocketTest();
	~WebSocketTest();

	uint64_t ClientContext(WebSocketClient& wsClient);
	void OnClientMessageReceived(const MessageEventArgs& messageEventArgs_);

protected:
	void SetUp() override;
	void TearDown() override {}

	WebSocketServer m_server;
};
