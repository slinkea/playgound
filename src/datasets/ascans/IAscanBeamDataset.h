#pragma once
#include "IAscanDataset.h"


class IAscanBeamDataset : public IAscanDataset
{
public:
  virtual ~IAscanBeamDataset() = default;
  
  virtual size_t BeamIndex() const = 0;
};
