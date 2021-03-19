#pragma once
#include "CscanBaseDataSet.hpp"
#include "ICscanBeamDataSet.h"


class CscanBeamDataSet : public CscanBaseDataSet, public ICscanBeamDataSet
{
public:
  CscanBeamDataSet(hid_t id_)
  : CscanBaseDataSet(id_)
  {
  }

  CscanBeamDataSet() = delete;
  CscanBeamDataSet(const CscanBeamDataSet&) = delete;
  CscanBeamDataSet& operator=(const CscanBeamDataSet&) = delete;
  virtual ~CscanBeamDataSet() = default;

  const std::string& Name() const override {
    return m_name;
  };

  size_t BeamIndex() const override {
    return m_beamIndex; 
  };

  const CscanAttributes& Attributes() const override {
    return m_attributes;
  }

  const CscanDataSpace& DataSpace() const override {
    return m_dataSpace;
  };

  void Read(const void* dataOut_) const override
  {
  }

protected:
  size_t m_beamIndex{};  
};
