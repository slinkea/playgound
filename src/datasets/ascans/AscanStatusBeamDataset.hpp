#pragma once
#include "IAscanBeamDataset.h"
#include "AscanStatusDataset.hpp"


class AscanStatusBeamDataset : public AscanStatusDataset, public IAscanBeamDataset
{
public:
  AscanStatusBeamDataset(hid_t Id_, const std::string& location_, size_t beamIdx_)
    : AscanStatusDataset(Id_, location_)
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
    return AscanStatusDataset::Location();
  };

  const AscanAttributes& Attributes() const override {
    return AscanStatusDataset::Attributes();
  }

  const AscanDataspace& Dataspace() const override {
    return AscanStatusDataset::Dataspace();
  };

  bool IsStatus() const {
    return AscanStatusDataset::IsStatus();
  }

  bool IsData() const {
    return AscanStatusDataset::IsData();
  }

  void Read(void* dataOut_) const override
  {
  }

protected:
  const size_t m_beamIdx{};
};
