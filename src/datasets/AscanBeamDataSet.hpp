#pragma once
#include "AscanDataset.hpp"
#include "IAscanBeamDataset.h"


class AscanBeamDataset : public AscanDataset, public IAscanBeamDataset
{
public:
  AscanBeamDataset(hid_t id_, const std::wstring& configName_, size_t beamIdx_)
    : AscanDataset(id_, configName_)
    , m_beamIdx(beamIdx_)
  {
  }

  AscanBeamDataset() = delete;
  AscanBeamDataset(const AscanBeamDataset&) = delete;
  AscanBeamDataset& operator=(const AscanBeamDataset&) = delete;
  virtual ~AscanBeamDataset() = default;

  const std::wstring& ConfigName() const override {
    return Dataset::ConfigName();
  };

  size_t BeamIndex() const override {
    return m_beamIdx;
  };

  const AscanAttributes& Attributes() const override {
    return AscanDataset::Attributes();
  }

  const AscanDataspace& Dataspace() const override {
    return AscanDataset::Dataspace();
  };

  void Read(const void* dataOut_) const override
  {
  }

protected:
  const size_t m_beamIdx{};  
};
