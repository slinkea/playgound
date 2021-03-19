#pragma once
#include "CscanDataSet.hpp"
#include "ICscanDataSet.h"


class CscanMergedBeamDataSet : public CscanDataSet, public ICscanDataSet
{
public:
  CscanMergedBeamDataSet(hid_t id_, const std::wstring& configName_, const std::wstring& sourceName_)
    : CscanDataSet(id_, configName_, sourceName_)
  {
  }

  CscanMergedBeamDataSet() = delete;
  CscanMergedBeamDataSet(const CscanMergedBeamDataSet&) = delete;
  CscanMergedBeamDataSet& operator=(const CscanMergedBeamDataSet&) = delete;

  virtual ~CscanMergedBeamDataSet() = default;

  const std::wstring& ConfigName() const override {
    return DataSet::ConfigName();
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
};
