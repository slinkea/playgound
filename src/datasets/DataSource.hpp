#pragma once
#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include "IDataset.h"


namespace fs = std::filesystem;

class DataSource
{
public:
  DataSource(const fs::path& filePath_, int configId_, const std::wstring& configName_,
    const std::vector<std::shared_ptr<const IDataset>>& datasets_)
    : m_filePath(filePath_)
    , m_configId(configId_)
    , m_configName(configName_)
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

  const std::vector<std::shared_ptr<const IDataset>> Datasets() const {
    return m_datasets;
  }

private:
  const int m_configId{};
  const fs::path m_filePath;
  const std::wstring m_configName;
  const std::vector<std::shared_ptr<const IDataset>> m_datasets;
};
