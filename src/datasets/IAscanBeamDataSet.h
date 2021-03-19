#pragma once
#include "IAscanDataSet.h"


class IAscanBeamDataSet : public IAscanDataSet
{
public:
  virtual ~IAscanBeamDataSet() = default;
  
  virtual size_t BeamIndex() const = 0;
};
