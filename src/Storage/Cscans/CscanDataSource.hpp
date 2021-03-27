#pragma once
#include "Storage/DataSource.hpp"


class CscanDataSource : public DataSource
{
public:
  CscanDataSource(const fs::path& filePath_, const TConfigIdPair& configIds_, bool beamDataMerged_)
    : DataSource(filePath_, configIds_)
    , m_beamDataMerged(beamDataMerged_)
  {
  }

  CscanDataSource(const CscanDataSource&) = default;
  virtual ~CscanDataSource() = default;

  CscanDataSource() = delete;
  CscanDataSource& operator=(const CscanDataSource&) = delete;

  bool IsBeamDataMerged() const {
    return false;
  }

private:
  bool m_beamDataMerged{};
};
