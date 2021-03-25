#pragma once
#include "datasets/Dataset.hpp"


class AscanStatusBeamDataset : public Dataset
{
public:
  AscanStatusBeamDataset(hid_t Id_, const std::string& location_, size_t beamIdx_)
    : Dataset(Id_, location_)
    , m_beamIdx(beamIdx_)
  {
  }

  AscanStatusBeamDataset(const AscanStatusBeamDataset&) = default;
  virtual ~AscanStatusBeamDataset() = default;

  AscanStatusBeamDataset() = delete;
  AscanStatusBeamDataset& operator=(const AscanStatusBeamDataset&) = delete;

private:
  size_t m_beamIdx{};
};
