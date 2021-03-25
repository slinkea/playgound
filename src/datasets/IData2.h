#pragma once
#include "DataSource2.hpp"

class IData2
{
public:
  virtual ~IData2() = default;

  virtual const DataSource2& Source() const = 0;

};
