#pragma once
#include "datasets/DataSource2.hpp"


class AscanDataSource2 : public DataSource2
{
public:
  AscanDataSource2(const fs::path& filePath_, size_t configId_)
    : DataSource2(filePath_, configId_)
  {
  }

  AscanDataSource2(const AscanDataSource2&) = default;
  virtual ~AscanDataSource2() = default;

  AscanDataSource2() = delete;
  AscanDataSource2& operator=(const AscanDataSource2&) = delete;

  size_t Id() const override {
    return m_configId;
  }

  const fs::path& FilePath() const override {
    return m_filePath;
  }

private:
  
};
