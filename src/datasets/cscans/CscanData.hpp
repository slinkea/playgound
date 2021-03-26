#pragma once
#include "datasets/IData.h"
#include "datasets/DatasetContainer.hpp"
#include "CscanDataSource.hpp"
#include "CscanDataset.hpp"
//#include "CscanBeamDataset.hpp"


class CscanData : public IData
{
public:
  CscanData(const CscanDataSource& dataSource_)
    : m_dataSource(dataSource_)
  {
  }

  CscanData(const CscanDataSource& dataSource_, DatasetContainer&& datasets_)
    : m_dataSource(dataSource_)
    , m_datasets(std::move(datasets_))
  {
  }

  CscanData() = default;
  virtual ~CscanData() = default;

  CscanData(const CscanData&) = delete;
  CscanData& operator=(const CscanData&) = delete;

  CscanData(CscanData&& other_) noexcept
    : m_datasets(std::move(other_.m_datasets))
    , m_dataSource(other_.m_dataSource)
  {
  }

  CscanData& operator=(CscanData&& other_) noexcept
  {
    if (this != &other_)
    {
      m_datasets = std::move(other_.m_datasets);
    }

    return *this;
  }

  const CscanDataSource& Source() const override {
    return m_dataSource;
  }

  const DatasetContainer& Datasets() const override {
    return m_datasets;
  }

  DatasetContainer& Datasets() override {
    return m_datasets;
  }

private:
  const CscanDataSource m_dataSource;
  DatasetContainer m_datasets;
};

using TCscanDataPtr = std::unique_ptr<CscanData>;
