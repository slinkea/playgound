#pragma once
#include <string>
#include <vector>
#include "DataSource.hpp"
#include "IAscanDataSource.h"


class AscanDataSource : public DataSource, public IAscanDataSource
{
public:
  AscanDataSource(const std::wstring& configName_, std::vector<const IDataset*> datasets_)
    : DataSource(configName_, datasets_)
  {
  }

  AscanDataSource() = delete;
  //AscanDataSource(const AscanDataSource&) = delete;
  AscanDataSource& operator=(const AscanDataSource&) = delete;

  virtual ~AscanDataSource() = default;

  const std::wstring& Name() const {
    return DataSource::Name();
  }

  const std::vector<const IDataset*> Datasets() const {
    return DataSource::Datasets();
  }

private:

};
