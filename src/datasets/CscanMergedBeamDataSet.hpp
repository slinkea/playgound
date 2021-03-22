#pragma once
#include "CscanDataset.hpp"
#include "ICscanDataset.h"


class CscanMergedBeamDataset : public CscanDataset, public ICscanDataset
{
public:
  CscanMergedBeamDataset(hid_t id_, const std::string& location_, const std::wstring& sourceName_)
    : CscanDataset(id_, location_, sourceName_)
  {
  }

  CscanMergedBeamDataset() = delete;
  CscanMergedBeamDataset(const CscanMergedBeamDataset&) = delete;
  CscanMergedBeamDataset& operator=(const CscanMergedBeamDataset&) = delete;

  virtual ~CscanMergedBeamDataset() = default;

  const std::string& Location() const override {
    return CscanDataset::Location();
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
