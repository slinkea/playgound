#pragma once
#include "AscanDataset.hpp"
#include "IAscanMergedBeamDataset.h"


class AscanMergedBeamDataset : public AscanDataset, public IAscanMergedBeamDataset
{
public:
  AscanMergedBeamDataset(hid_t dataId_, hid_t statusId_, const std::wstring& configName_)
    : AscanDataset(dataId_, statusId_, configName_)
  {
  }

  AscanMergedBeamDataset() = delete;
  AscanMergedBeamDataset(const AscanMergedBeamDataset&) = delete;
  AscanMergedBeamDataset& operator=(const AscanMergedBeamDataset&) = delete;

  virtual ~AscanMergedBeamDataset() = default;

  const std::wstring& Configuration() const override {
    return AscanDataset::Configuration();
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
