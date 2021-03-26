#pragma once
#include "AscanStatusDataset.hpp"


class AscanStatusBeamDataset : public AscanStatusDataset
{
public:
  AscanStatusBeamDataset(hid_t Id_, const std::string& location_, size_t beamIdx_)
    : AscanStatusDataset(Id_, location_)
    , m_beamIdx(beamIdx_)
  {
  }

  AscanStatusBeamDataset(const AscanStatusBeamDataset&) = default;
  virtual ~AscanStatusBeamDataset() = default;

  AscanStatusBeamDataset() = delete;
  AscanStatusBeamDataset& operator=(const AscanStatusBeamDataset&) = delete;

  size_t BeamIndex() const {
    return m_beamIdx;
  }

private:
  const size_t m_beamIdx{};
};
