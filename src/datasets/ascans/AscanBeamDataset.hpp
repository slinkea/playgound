#pragma once
#include "AscanDatasetBase.hpp"


class AscanBeamDataset : public AscanDatasetBase
{
public:
  AscanBeamDataset(hid_t Id_, const std::string& location_, size_t beamIdx_)
    : AscanDatasetBase(Id_, location_)
    , m_beamIdx(beamIdx_)
  {
  }

  AscanBeamDataset(const AscanBeamDataset&) = default;
  virtual ~AscanBeamDataset() = default;

  AscanBeamDataset() = delete;
  AscanBeamDataset& operator=(const AscanBeamDataset&) = delete;

private:
  size_t m_beamIdx{};
};
