#pragma once
#include "CscanDataset.hpp"
#include "Storage/DatasetDefinition.h"


class CscanBeamDataset : public CscanDataset
{
public:
  CscanBeamDataset(hid_t dsetId_, const TGateIdentifiers& gateIds_, size_t beamIdx_)
    : CscanDataset(dsetId_, gateIds_)
    , m_beamIdx(beamIdx_)
  {
  }

  CscanBeamDataset(const CscanBeamDataset&) = default;
  virtual ~CscanBeamDataset() = default;

  CscanBeamDataset() = delete;
  CscanBeamDataset& operator=(const CscanBeamDataset&) = delete;

  size_t BeamIndex() const {
    return m_beamIdx;
  }

private:
  const size_t m_beamIdx{};
};
