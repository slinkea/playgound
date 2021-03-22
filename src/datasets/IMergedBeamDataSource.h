#pragma once
#include "IDataSource.h"


class IMergedBeamDataSource : public IDataSource
{
public:
  virtual ~IMergedBeamDataSource() = default;

  virtual size_t BeamQuantity() const = 0;
};
