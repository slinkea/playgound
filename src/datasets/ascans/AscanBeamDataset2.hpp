#pragma once
#include "datasets/Dataset2.hpp"


class AscanBeamDataset2 : public Dataset2
{
public:
  AscanBeamDataset2(hid_t Id_, const std::string& location_, size_t beamIdx_)
    : Dataset2(Id_, location_)
    , m_beamIdx(beamIdx_)
  {
  }

  AscanBeamDataset2(const AscanBeamDataset2&) = default;
  virtual ~AscanBeamDataset2() = default;

  AscanBeamDataset2() = delete;
  AscanBeamDataset2& operator=(const AscanBeamDataset2&) = delete;

  const AscanAttributes& Attributes() const {
    return m_attributes;
  }

private:
  size_t m_beamIdx{};
  AscanAttributes m_attributes;
};
