#pragma once
#include "datasets/Dataset.hpp"


class AscanBeamDataset : public Dataset
{
public:
  AscanBeamDataset(hid_t Id_, const std::string& location_, size_t beamIdx_)
    : Dataset(Id_, location_)
    , m_beamIdx(beamIdx_)
  {
  }

  AscanBeamDataset(const AscanBeamDataset&) = default;
  virtual ~AscanBeamDataset() = default;

  AscanBeamDataset() = delete;
  AscanBeamDataset& operator=(const AscanBeamDataset&) = delete;

  const AscanAttributes& Attributes() const {
    return m_attributes;
  }

private:
  size_t m_beamIdx{};
  AscanAttributes m_attributes;
};
