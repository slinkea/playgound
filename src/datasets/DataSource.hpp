#pragma once
#include <string>
#include <vector>
#include "IDataset.h"


class DataSource
{
public:
  DataSource(const std::wstring& configName_, std::vector<const IDataset*> datasets_)
    : m_configName(configName_)
    , m_datasets(datasets_)
  {
  }

  DataSource() = delete;
  //DataSource(const DataSource&) = delete;
  DataSource& operator=(const DataSource&) = delete;

  virtual ~DataSource() = default;

  const std::wstring& Name() const {
    return m_configName;
  }

  const std::vector<const IDataset*> Datasets() const {
    return m_datasets;
  }

private:
  const std::wstring m_configName;
  const std::vector<const IDataset*> m_datasets;
};
