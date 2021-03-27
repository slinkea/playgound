#pragma once
#include <map>
#include <string>
#include <filesystem>


namespace fs = std::filesystem;

class DataSource
{
public:
  DataSource(const fs::path& filePath_, const TConfigIdPair& configIds_)
    : m_filePath(filePath_)
    , m_configIds(configIds_)
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
    return m_configIds.second;
  }

  size_t ConfigId() const {
    return m_configIds.first;
  }

protected:
  const fs::path m_filePath;
  const TConfigIdPair m_configIds;
};
