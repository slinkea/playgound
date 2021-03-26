#pragma once
#include <string>
#include "DatasetDefinition.h"
#include "DatasetProperties.hpp"


class IDataset
{
public:
  virtual ~IDataset() = default;

  virtual const DataDimensions& Dimensions() const = 0;
  virtual const DatasetProperties& Properties() const = 0;
  virtual void Read(void* dataOut_) const = 0;
};
