#pragma once
#include "AscanDataset.hpp"


class AscanBeamDataset : public AscanDataset
{
public:
  AscanBeamDataset(hid_t Id_, const std::string& location_, size_t beamIdx_)
    : AscanDataset(Id_, location_)
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
