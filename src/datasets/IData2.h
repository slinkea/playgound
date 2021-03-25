#pragma once
#include "DataSource2.hpp"
#include "datasets/DatasetContainer2.hpp"

class IData2
{
public:
  virtual ~IData2() = default;

  virtual const DataSource2& Source() const = 0;
  virtual const DatasetContainer2& Datasets() const = 0;
  virtual DatasetContainer2& Datasets() = 0;
};
