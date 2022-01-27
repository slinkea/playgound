#pragma once
#include <gtest/gtest.h>


class HDF5AscanTest : public testing::Test
{
public:
	HDF5AscanTest();
	virtual ~HDF5AscanTest();

protected:
	void SetUp() override;
	void TearDown() override;


};
