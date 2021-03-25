#pragma once
#include "AscanDatasetBase.hpp"
#include "datasets/DatasetDefinition.h"


class AscanDataset : public AscanDatasetBase
{
public:
  AscanDataset(hid_t dsetId_, const std::string& location_)
    : AscanDatasetBase(dsetId_, location_)
  {
  }

  AscanDataset(const AscanDataset&) = default;
  virtual ~AscanDataset() = default;

  AscanDataset() = delete;
  AscanDataset& operator=(const AscanDataset&) = delete;
};
