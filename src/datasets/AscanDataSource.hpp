#pragma once
#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include "DataSource.hpp"
#include "IAscanDataSource.h"


namespace fs = std::filesystem;

class AscanDataSource : public DataSource, public IAscanDataSource
{
public:
  AscanDataSource(const fs::path& filePath_, int configId_, const std::wstring& configName_, 
    const std::vector<std::shared_ptr<const IDataset>>& datasets_)
    : DataSource(filePath_, configId_, configName_, datasets_)
  {
  }

  AscanDataSource() = delete;
  //AscanDataSource(const AscanDataSource&) = delete;
  AscanDataSource& operator=(const AscanDataSource&) = delete;

  virtual ~AscanDataSource() = default;

  const std::wstring& Name() const override {
    return DataSource::Name();
  }

  const std::vector<std::shared_ptr<const IDataset>> Datasets() const override {
    return DataSource::Datasets();
  }

private:

};
