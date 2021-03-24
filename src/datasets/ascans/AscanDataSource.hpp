#pragma once
#include <string>
#include <filesystem>
#include "datasets/DataSource.hpp"
#include "IAscanDataSource.h"


namespace fs = std::filesystem;

class AscanDataSource : public DataSource, public IAscanDataSource
{
public:
  AscanDataSource(const fs::path& filePath_, size_t configId_, const std::wstring& configName_)
    : DataSource(filePath_, configId_, configName_)
  {
  }

  AscanDataSource() = delete;
  //AscanDataSource(const AscanDataSource&) = delete;
  AscanDataSource& operator=(const AscanDataSource&) = delete;

  virtual ~AscanDataSource() = default;

  size_t Id() const override {
    return DataSource::Id();
  }

  const std::wstring& Name() const override {
    return DataSource::Name();
  }

private:

};
