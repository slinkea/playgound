#pragma once
#include "IDataSource.h"


class IReadOnlyData
{
public:
  virtual ~IReadOnlyData() = default;

  virtual const IDataSource* Source() const = 0;

};
