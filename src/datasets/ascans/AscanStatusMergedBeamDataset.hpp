#pragma once
#include "AscanStatusDataset.hpp"
#include "IAscanMergedBeamDataset.h"


class AscanStatusMergedBeamDataset : public AscanStatusDataset, public IAscanMergedBeamDataset
{
public:
  AscanStatusMergedBeamDataset(hid_t Id_, const std::string& location_)
    : AscanStatusDataset(Id_, location_)
  {
  }

  AscanStatusMergedBeamDataset() = delete;
  AscanStatusMergedBeamDataset(const AscanStatusMergedBeamDataset&) = delete;
  AscanStatusMergedBeamDataset& operator=(const AscanStatusMergedBeamDataset&) = delete;

  virtual ~AscanStatusMergedBeamDataset() = default;

  const std::string& Location() const override {
    return AscanStatusDataset::Location();
  };

  const AscanAttributes& Attributes() const override {
    return AscanStatusDataset::Attributes();
  }

  const AscanDataspace& Dataspace() const override {
    return AscanStatusDataset::Dataspace();
  };

  bool IsStatus() const {
    return AscanStatusDataset::IsStatus();
  }

  bool IsData() const {
    return AscanStatusDataset::IsData();
  }

  void Read(void* dataOut_) const override
  {
  }
};
