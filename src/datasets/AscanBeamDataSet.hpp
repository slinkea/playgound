#pragma once
#include "AscanDataSet.hpp"
#include "IAscanBeamDataSet.h"


class AscanBeamDataSet : public AscanDataSet, public IAscanBeamDataSet
{
public:
  AscanBeamDataSet(hid_t id_, const std::wstring& configName_, size_t beamIdx_)
    : AscanDataSet(id_, configName_)
    , m_beamIdx(beamIdx_)
  {
  }

  AscanBeamDataSet() = delete;
  AscanBeamDataSet(const AscanBeamDataSet&) = delete;
  AscanBeamDataSet& operator=(const AscanBeamDataSet&) = delete;
  virtual ~AscanBeamDataSet() = default;

  const std::wstring& ConfigName() const override {
    return DataSet::ConfigName();
  };

  size_t BeamIndex() const override {
    return m_beamIdx;
  };

  const AscanAttributes& Attributes() const override {
    return AscanDataSet::Attributes();
  }

  const AscanDataSpace& DataSpace() const override {
    return AscanDataSet::DataSpace();
  };

  void Read(const void* dataOut_) const override
  {
  }

protected:
  const size_t m_beamIdx{};  
};
