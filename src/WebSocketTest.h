#pragma once
#include <gtest/gtest.h>

#include "WebSocketServer.h"
#include "WebSocketClient.h"


class WebSocketTest : public testing::Test
{
public:
	WebSocketTest();
	virtual ~WebSocketTest();

	void Run(WebSocketClient& wsClient, std::atomic<uint64_t>& checksum_);

protected:
	void SetUp() override;
	void TearDown() override;

	WebSocketServer m_server;
};
