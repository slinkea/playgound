#pragma once
#include "IAscanData.h"
#include "AscanDataSource.hpp"
#include "IAscanDatasetVector.hpp"


class AscanData : public IAscanData
{
public:
  AscanData(const AscanDataSource& dataSource_, const IAscanDatasetVector& datasets_)
    : m_dataSource(dataSource_)
    , m_datasets(datasets_)
  {
  }

  AscanData() = delete;
  AscanData(const AscanData&) = delete;
  AscanData& operator=(const AscanData&) = delete;

  virtual ~AscanData() = default;

  const IAscanDataSource* Source() const override {
    return &m_dataSource;
  }

  const IAscanDatasetVector& Datasets() const override {
    return m_datasets;
  }

  void Read(void* dataOut_) const override {
    throw 0;
  }



private:
  const IAscanDatasetVector m_datasets;
  const AscanDataSource m_dataSource;
};
