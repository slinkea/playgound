#pragma once
#include "IAscanData.h"
#include "AscanDataSource.hpp"


class AscanData : public IAscanData
{
public:
  AscanData(const AscanDataSource& dataSource_)
    : m_dataSource(dataSource_)
  {
    auto x = m_dataSource.Datasets();
  }

  AscanData() = delete;
  AscanData(const AscanData&) = delete;
  AscanData& operator=(const AscanData&) = delete;

  virtual ~AscanData() = default;

  const IAscanDataSource* Source() const override {
    return &m_dataSource;
  }

  const AscanAttributes& Attributes() const override {
    throw 0;
  }

  const AscanDataspace& Dataspace() const override {
    throw 0;
  }

  void Read(void* dataOut_) const override {
    throw 0;
  }

private:
  const AscanDataSource m_dataSource;
};
