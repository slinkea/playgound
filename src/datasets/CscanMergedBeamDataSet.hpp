#pragma once
#include "CscanDataset.hpp"
#include "ICscanDataset.h"


class CscanMergedBeamDataset : public CscanDataset, public ICscanDataset
{
public:
  CscanMergedBeamDataset(hid_t id_, const std::wstring& configName_, const std::wstring& sourceName_)
    : CscanDataset(id_, configName_, sourceName_)
  {
  }

  CscanMergedBeamDataset() = delete;
  CscanMergedBeamDataset(const CscanMergedBeamDataset&) = delete;
  CscanMergedBeamDataset& operator=(const CscanMergedBeamDataset&) = delete;

  virtual ~CscanMergedBeamDataset() = default;

  const std::wstring& ConfigName() const override {
    return Dataset::ConfigName();
  };

  const CscanAttributes& Attributes() const override {
    return CscanDataset::Attributes();
  }

  const CscanDataspace& Dataspace() const override {
    return CscanDataset::Dataspace();
  };

  void Read(const void* dataOut_) const override
  {
  }
};
