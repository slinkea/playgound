#pragma once
#include "Storage/IData.h"
#include "AscanDatasetContainer.hpp"
#include "AscanDataSource.hpp"
#include "AscanDataset.hpp"
#include "AscanBeamDataset.hpp"


class AscanData : public IData
{
public:
  AscanData(const AscanDataSource& dataSource_)
    : m_dataSource(dataSource_)
  {
  }

  AscanData(const AscanDataSource& dataSource_, AscanDatasetContainer&& datasets_)
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

  const AscanDatasetContainer& Datasets() const {
    return m_datasets;
  }

  AscanDatasetContainer& Datasets() {
    return m_datasets;
  }

  bool IsDataMerged() const
  {
    return !std::all_of(m_datasets.Items().begin(), m_datasets.Items().end(), [](const auto& item) {
      return dynamic_cast<const AscanBeamDataset*>(item.get());
      });
  }

private:
  const AscanDataSource m_dataSource;
  AscanDatasetContainer m_datasets;
};

using TAscanDataPtr = std::unique_ptr<AscanData>;
