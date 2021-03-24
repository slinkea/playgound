#pragma once
#include "IAscanBeamDataset.h"
#include "AscanDataset.hpp"


class AscanStatusBeamDataset : public AscanDataset, public IAscanBeamDataset
{
public:
  AscanStatusBeamDataset(hid_t Id_, const std::string& location_, size_t beamIdx_)
    : AscanDataset(Id_, location_)
  {
  }

  AscanStatusBeamDataset() = delete;
  AscanStatusBeamDataset(const AscanStatusBeamDataset&) = delete;
  AscanStatusBeamDataset& operator=(const AscanStatusBeamDataset&) = delete;

  virtual ~AscanStatusBeamDataset() = default;

  size_t BeamIndex() const override {
    return m_beamIdx;
  };

  const std::string& Location() const {
    return AscanDataset::Location();
  };

  const AscanAttributes& Attributes() const override {
    return AscanDataset::Attributes();
  }

  const DataDimensions& Dimensions() const override  {
    return AscanDataset::Dimensions();
  }

  const DatasetProperties& Properties() const override {
    return AscanDataset::Properties();
  }

  bool IsStatus() const override {
    return true;
  }

  bool IsData() const override {
    return false;
  }

  void Read(void* dataOut_) const override
  {
  }

protected:
  const size_t m_beamIdx{};
};
