#pragma once
#include "AscanDataSource2.hpp"
#include "datasets/DatasetContainer2.hpp"


class AscanData2 : public IData2
{
public:
  AscanData2(const AscanDataSource2& dataSource_, DatasetContainer2&& datasets_)
    : m_dataSource(dataSource_)
    , m_datasets(std::move(datasets_))
  {
  }

  virtual ~AscanData2() = default;

  AscanData2() = delete;
  AscanData2(const AscanData2&) = delete;
  AscanData2& operator=(const AscanData2&) = delete;

  AscanData2(AscanData2&& other_) noexcept
    : m_datasets(std::move(other_.m_datasets))
    , m_dataSource(other_.m_dataSource)
  {
  }

  AscanData2& operator=(AscanData2&& other_) noexcept
  {
    if (this != &other_)
    {
      m_datasets = std::move(other_.m_datasets);
    }

    return *this;
  }

  const AscanDataSource2& Source() const override {
    return m_dataSource;
  }

  const DatasetContainer2& Datasets() const override {
    return m_datasets;
  }

  DatasetContainer2& Datasets() override {
    return m_datasets;
  }

private:
  const AscanDataSource2 m_dataSource;
  DatasetContainer2 m_datasets;
};
