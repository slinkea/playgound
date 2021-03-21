#pragma once
#include "AscanDataset.hpp"
#include "IAscanMergedBeamDataset.h"


class AscanMergedBeamDataset : public AscanDataset, public IAscanMergedBeamDataset
{
public:
  AscanMergedBeamDataset(hid_t id_, const std::wstring& configName_)
    : AscanDataset(id_, configName_)
  {
  }

  AscanMergedBeamDataset() = delete;
  AscanMergedBeamDataset(const AscanMergedBeamDataset&) = delete;
  AscanMergedBeamDataset& operator=(const AscanMergedBeamDataset&) = delete;

  virtual ~AscanMergedBeamDataset() = default;

  const std::wstring& ConfigName() const override {
    return Dataset::ConfigName();
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
};
