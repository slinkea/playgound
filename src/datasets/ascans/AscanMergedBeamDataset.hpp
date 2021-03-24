#pragma once
#include "AscanDataset.hpp"
#include "IAscanMergedBeamDataset.h"


class AscanMergedBeamDataset : public AscanDataset, public IAscanMergedBeamDataset
{
public:
  AscanMergedBeamDataset(hid_t Id_, const std::string& location_)
    : AscanDataset(Id_, location_)
  {
  }

  AscanMergedBeamDataset() = delete;
  AscanMergedBeamDataset(const AscanMergedBeamDataset&) = delete;
  AscanMergedBeamDataset& operator=(const AscanMergedBeamDataset&) = delete;

  virtual ~AscanMergedBeamDataset() = default;

  const std::string& Location() const override {
    return AscanDataset::Location();
  };

  const AscanAttributes& Attributes() const override {
    return AscanDataset::Attributes();
  }

  const AscanDataspace& Dataspace() const override {
    return AscanDataset::Dataspace();
  };

  bool IsStatus() const {
    return AscanDataset::IsStatus();
  }

  bool IsData() const {
    return AscanDataset::IsData();
  }

  void Read(void* dataOut_) const override
  {
  }
};
