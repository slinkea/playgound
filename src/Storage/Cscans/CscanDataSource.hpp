#pragma once
#include "Storage/DataSource.hpp"


class CscanDataSource : public DataSource
{
public:
  CscanDataSource(const fs::path& filePath_, size_t configId_, const std::wstring& configName_, const TGateIdentifiers& gateIds_)
    : DataSource(filePath_, configId_, configName_)
    , m_gateIds(gateIds_)
  {
  }

  CscanDataSource(const CscanDataSource&) = default;
  virtual ~CscanDataSource() = default;

  CscanDataSource() = delete;
  CscanDataSource& operator=(const CscanDataSource&) = delete;

private:
  const TGateIdentifiers& m_gateIds;
};
