#pragma once
#include "AscanDataset.hpp"
#include "IAscanMergedBeamDataset.h"


class AscanMergedBeamStatusDataset //: public AscanDataset, public IAscanMergedBeamDataset
{
public:
  AscanMergedBeamStatusDataset(hid_t Id_, const std::string& location_)
    : AscanDataset(Id_, location_)
  {
  }

  AscanMergedBeamStatusDataset() = delete;
  AscanMergedBeamStatusDataset(const AscanMergedBeamStatusDataset&) = delete;
  AscanMergedBeamStatusDataset& operator=(const AscanMergedBeamStatusDataset&) = delete;

  virtual ~AscanMergedBeamStatusDataset() = default;

  const std::string& Location() const override {
    return AscanDataset::Location();
  };

  const AscanDataspace& Dataspace() const override {
    return AscanDataset::Dataspace();
  };

  void Read(const void* dataOut_) const override
  {
  }
};
