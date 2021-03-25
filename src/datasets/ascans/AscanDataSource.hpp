#pragma once
#include "datasets/DataSource.hpp"


class AscanDataSource : public DataSource
{
public:
  AscanDataSource(const fs::path& filePath_, size_t configId_, const std::wstring& configName_)
    : DataSource(filePath_, configId_, configName_)
  {
  }

  AscanDataSource(const AscanDataSource&) = default;
  virtual ~AscanDataSource() = default;

  AscanDataSource() = delete;
  AscanDataSource& operator=(const AscanDataSource&) = delete;
};
