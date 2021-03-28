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

  CscanDataSource() = default;
  CscanDataSource(const CscanDataSource&) = default;
  virtual ~CscanDataSource() = default;

  CscanDataSource& operator=(const CscanDataSource&) = delete;

  bool IsBeamDataMerged() const {
    return m_beamDataMerged;
  }

private:
  bool m_beamDataMerged{};
};
