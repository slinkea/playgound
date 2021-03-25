#pragma once
#include "datasets/IDataSource2.h"


class IAscanDataSource2
{
public:
  virtual ~IAscanDataSource2() = default;

  virtual size_t Id() const = 0;
};
