#pragma once
#include <gtest/gtest.h>


class PlaygroundTest : public testing::Test
{
public:
	PlaygroundTest();
	virtual ~PlaygroundTest();

private:

protected:
	void SetUp() override;
	void TearDown() override;
};
