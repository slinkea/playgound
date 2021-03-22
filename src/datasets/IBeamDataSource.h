#pragma once
#include "IDataSource.h"


class IBeamDataSource : public IDataSource
{
public:
  virtual ~IBeamDataSource() = default;
  
  virtual size_t BeamIndex() const = 0;
};
