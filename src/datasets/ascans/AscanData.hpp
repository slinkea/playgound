#pragma once
#include "datasets/DatasetContainer.hpp"
#include "AscanDataSource.hpp"
#include "AscanDataset.hpp"
#include "AscanBeamDataset.hpp"
#include "AscanStatusDataset.hpp"
#include "AscanStatusBeamDataset.hpp"


class AscanData : public IData
{
public:
  AscanData(const AscanDataSource& dataSource_)
    : m_dataSource(dataSource_)
  {
  }

  AscanData(const AscanDataSource& dataSource_, DatasetContainer&& datasets_)
    : m_dataSource(dataSource_)
    , m_datasets(std::move(datasets_))
  {
  }

  AscanData() = default;
  virtual ~AscanData() = default;

  AscanData(const AscanData&) = delete;
  AscanData& operator=(const AscanData&) = delete;

  AscanData(AscanData&& other_) noexcept
    : m_datasets(std::move(other_.m_datasets))
    , m_dataSource(other_.m_dataSource)
  {
  }

  AscanData& operator=(AscanData&& other_) noexcept
  {
    if (this != &other_)
    {
      m_datasets = std::move(other_.m_datasets);
    }

    return *this;
  }

  const AscanDataSource& Source() const override {
    return m_dataSource;
  }

  const DatasetContainer& Datasets() const override {
    return m_datasets;
  }

  DatasetContainer& Datasets() override {
    return m_datasets;
  }

private:
  const AscanDataSource m_dataSource;
  DatasetContainer m_datasets;
};

using TAscanDataPtr = std::unique_ptr<AscanData>;
