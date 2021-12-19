#pragma once
#include <gtest/gtest.h>


class HDF5Test : public testing::Test
{
public:
	HDF5Test();
	virtual ~HDF5Test();

private:

protected:
	void SetUp() override;
	void TearDown() override;
};
