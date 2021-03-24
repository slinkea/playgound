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

  const std::string& Location() const {
    return AscanDataset::Location();
  };

  const AscanAttributes& Attributes() const override {
    return AscanDataset::Attributes();
  }

  const DataDimensions& Dimensions() const override {
    return AscanDataset::Dimensions();
  }

  const DatasetProperties& Properties() const override {
    return AscanDataset::Properties();
  }

  bool IsStatus() const override {
    return false;
  }

  bool IsData() const override {
    return true;
  }

  void Read(void* dataOut_) const override
  {
  }
};
