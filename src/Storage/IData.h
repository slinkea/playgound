#pragma once
#include "DataSource.hpp"
#include "DatasetContainer.hpp"

class IData
{
public:
  virtual ~IData() = default;

  virtual const DataSource& Source() const = 0;
  virtual const DatasetContainer& Datasets() const = 0;
  virtual DatasetContainer& Datasets() = 0;
};
