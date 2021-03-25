#pragma once
#include "AscanDataset.hpp"
#include "IAscanMergedBeamDataset.h"


class AscanStatusMergedBeamDataset : public AscanDataset, public IAscanMergedBeamDataset
{
public:
  AscanStatusMergedBeamDataset(hid_t Id_, const std::string& location_)
    : AscanDataset(Id_, location_)
  {
  }

  AscanStatusMergedBeamDataset() = delete;
  AscanStatusMergedBeamDataset(const AscanStatusMergedBeamDataset&) = delete;
  AscanStatusMergedBeamDataset& operator=(const AscanStatusMergedBeamDataset&) = delete;

  virtual ~AscanStatusMergedBeamDataset() = default;

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
    return true;
  }

  bool IsData() const override {
    return false;
  }

  void SelectSingle(size_t x_, size_t y_) const override {
    return AscanDataset::SelectSingle(x_, y_);
  }

  void Read(void* dataOut_) const override
  {
  }
};
