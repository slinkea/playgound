#pragma once
#include "AscanDataset.hpp"
#include "IAscanBeamDataset.h"


class AscanBeamDataset : public AscanDataset, public IAscanBeamDataset
{
public:
  AscanBeamDataset(hid_t Id_, const std::string& location_, size_t beamIdx_)
    : AscanDataset(Id_, location_)
    , m_beamIdx(beamIdx_)
  {
  }

  AscanBeamDataset() = delete;
  AscanBeamDataset(const AscanBeamDataset&) = delete;
  AscanBeamDataset& operator=(const AscanBeamDataset&) = delete;
  virtual ~AscanBeamDataset() = default;

  const std::string& Location() const override {
    return AscanDataset::Location();
  };

  size_t BeamIndex() const override {
    return m_beamIdx;
  };

  const AscanAttributes& Attributes() const override {
    return AscanDataset::Attributes();
  }

  const AscanDataspace& Dataspace() const override {
    return AscanDataset::Dataspace();
  }

  bool IsStatus() const {
    return AscanDataset::IsStatus();
  }

  bool IsData() const {
    return AscanDataset::IsData();
  }

  void Read(void* dataOut_) const override
  {
  }

protected:
  const size_t m_beamIdx{};  
};
