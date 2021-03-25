#pragma once
#include <string>
#include <filesystem>


namespace fs = std::filesystem;

class DataSource2
{
public:
  DataSource2(const fs::path& filePath_, size_t configId_, const std::wstring& configName_)
    : m_filePath(filePath_)
    , m_configId(configId_)
    , m_configName(configName_)
  {
  }

  DataSource2(const DataSource2&) = default;
  virtual ~DataSource2() = default;

  DataSource2() = delete;
  DataSource2& operator=(const DataSource2&) = delete;

  virtual const fs::path& FilePath() const = 0;

  virtual size_t Id() const = 0;

protected:
  const fs::path m_filePath;
  const size_t m_configId;
  const std::wstring m_configName;
};
