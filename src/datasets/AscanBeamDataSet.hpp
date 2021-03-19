#pragma once
#include "AscanBaseDataSet.hpp"
#include "IAscanBeamDataSet.h"


class AscanBeamDataSet : public AscanBaseDataSet, public IAscanBeamDataSet
{
public:
  AscanBeamDataSet(hid_t id_, size_t beamIndex_)
  : AscanBaseDataSet(id_)
  , m_beamIndex(beamIndex_)
  {
  }

  AscanBeamDataSet() = delete;
  AscanBeamDataSet(const AscanBeamDataSet&) = delete;
  AscanBeamDataSet& operator=(const AscanBeamDataSet&) = delete;
  virtual ~AscanBeamDataSet() = default;

  const std::string& Name() const override {
    return m_name;
  };

  size_t BeamIndex() const override {
    return m_beamIndex; 
  };

  const AscanAttributes& Attributes() const override {
    return m_attributes;
  }

  const AscanDataSpace& DataSpace() const override {
    return m_dataSpace;
  };

  void Read(const void* dataOut_) const override
  {
  }

protected:
  size_t m_beamIndex{};  
};
