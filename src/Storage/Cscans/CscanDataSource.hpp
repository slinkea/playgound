#pragma once
#include "Storage/DataSource.hpp"


class CscanDataSource : public DataSource
{
public:
  CscanDataSource(const fs::path& filePath_, size_t configId_, const std::wstring& configName_)
    : DataSource(filePath_, configId_, configName_)
  {
  }

  CscanDataSource(const CscanDataSource&) = default;
  virtual ~CscanDataSource() = default;

  CscanDataSource() = delete;
  CscanDataSource& operator=(const CscanDataSource&) = delete;

  bool IsBeamDataMerged() const {
    return false;
  }
};
