#pragma once
#include <string>
#include <filesystem>


namespace fs = std::filesystem;

class DataSource
{
public:
  DataSource(const fs::path& filePath_, size_t configId_, const std::wstring& configName_)
    : m_filePath(filePath_)
    , m_configId(configId_)
    , m_configName(configName_)
  {
  }

  DataSource(const DataSource&) = default;
  virtual ~DataSource() = default;

  DataSource() = delete;
  DataSource& operator=(const DataSource&) = delete;

  const fs::path& FilePath() const {
    return m_filePath;
  }

  const std::wstring& ConfigName() const {
    return m_configName;
  }

  size_t ConfigId() const {
    return m_configId;
  }

protected:
  const size_t m_configId;
  const fs::path m_filePath;
  const std::wstring m_configName;
};
