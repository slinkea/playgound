#pragma once
#include "Storage/IData.h"
#include "CscanDatasetContainer.hpp"
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

  CscanData(const CscanDataSource& dataSource_, CscanDatasetContainer&& datasets_)
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

  const CscanDatasetContainer& Datasets() const override {
    return m_datasets;
  }

  CscanDatasetContainer& Datasets() override {
    return m_datasets;
  }

private:
  const CscanDataSource m_dataSource;
  CscanDatasetContainer m_datasets;
};

using TCscanDataPtr = std::unique_ptr<CscanData>;
