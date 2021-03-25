#pragma once
#include "datasets/Dataset2.hpp"


class AscanStatusBeamDataset2 : public Dataset2
{
public:
  AscanStatusBeamDataset2(hid_t Id_, const std::string& location_, size_t beamIdx_)
    : Dataset2(Id_, location_)
    , m_beamIdx(beamIdx_)
  {
  }

  AscanStatusBeamDataset2(const AscanStatusBeamDataset2&) = default;
  virtual ~AscanStatusBeamDataset2() = default;

  AscanStatusBeamDataset2() = delete;
  AscanStatusBeamDataset2& operator=(const AscanStatusBeamDataset2&) = delete;

private:
  size_t m_beamIdx{};
};
