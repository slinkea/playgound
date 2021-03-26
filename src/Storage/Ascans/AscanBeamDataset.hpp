#pragma once
#include "AscanDataset.hpp"
#include "Storage/DatasetDefinition.h"


class AscanBeamDataset : public AscanDataset
{
public:
  AscanBeamDataset(const TDatasetKeys& dsetKeys_, size_t beamIdx_)
    : AscanDataset(dsetKeys_)
    , m_beamIdx(beamIdx_)
  {
  }

  AscanBeamDataset(const AscanBeamDataset&) = default;
  virtual ~AscanBeamDataset() = default;

  AscanBeamDataset() = delete;
  AscanBeamDataset& operator=(const AscanBeamDataset&) = delete;

  size_t BeamIndex() const {
    return m_beamIdx;
  }

private:
  const size_t m_beamIdx{};
};
