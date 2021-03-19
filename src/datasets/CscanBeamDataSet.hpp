#pragma once
#include "CscanDataSet.hpp"
#include "ICscanBeamDataSet.h"


class CscanBeamDataSet : public CscanDataSet, public ICscanBeamDataSet
{
public:
  CscanBeamDataSet(hid_t id_, const std::wstring& configName_, size_t beamIdx_, const std::wstring& sourceName_)
    : CscanDataSet(id_, configName_, sourceName_)
    , m_beamIdx(beamIdx_)
  {
  }

  CscanBeamDataSet() = delete;
  CscanBeamDataSet(const CscanBeamDataSet&) = delete;
  CscanBeamDataSet& operator=(const CscanBeamDataSet&) = delete;
  virtual ~CscanBeamDataSet() = default;

  const std::wstring& ConfigName() const override {
    return DataSet::ConfigName();
  };

  size_t BeamIndex() const override {
    return m_beamIdx;
  };

  const CscanAttributes& Attributes() const override {
    return CscanDataSet::Attributes();
  }

  const CscanDataSpace& DataSpace() const override {
    return CscanDataSet::DataSpace();
  };

  void Read(const void* dataOut_) const override
  {
  }

protected:
  size_t m_beamIdx{};
};
