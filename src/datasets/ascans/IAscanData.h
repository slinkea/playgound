#pragma once
#include "datasets/IData.h"
#include "IAscanDataset.h"
#include "IAscanDataSource.h"
#include "IAscanDatasetVector.hpp"


class AscanDataspace;
struct AscanAttributes;

class IAscanData : public IData
{
public:
  virtual ~IAscanData() = default;

  virtual const IAscanDataSource* Source() const = 0;
  virtual const IAscanDatasetVector& Datasets() const = 0;

};
