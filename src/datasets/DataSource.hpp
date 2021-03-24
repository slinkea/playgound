#pragma once
#include <string>
#include <filesystem>


namespace fs = std::filesystem;

class DataSource
{
public:
  DataSource(const fs::path& filePath_, int configId_, const std::wstring& configName_)
    : m_filePath(filePath_)
    , m_configId(configId_)
    , m_configName(configName_)
  {
  }

  DataSource() = delete;
  //DataSource(const DataSource&) = delete;
  DataSource& operator=(const DataSource&) = delete;

  virtual ~DataSource() = default;

  size_t Id() const {
    return m_configId;
  }

  const std::wstring& Name() const {
    return m_configName;
  }

private:
  const int m_configId{};
  const fs::path m_filePath;
  const std::wstring m_configName;
};
