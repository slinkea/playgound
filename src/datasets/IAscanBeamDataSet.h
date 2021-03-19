#pragma once
#include "IDataSet.h"


struct AscanAttributes;
class AscanDataSpace;

class IAscanBeamDataSet : public IDataSet
{
public:
  virtual ~IAscanBeamDataSet() = default;
  
  virtual size_t BeamIndex() const = 0;
  virtual const AscanAttributes& Attributes() const = 0;
  virtual const AscanDataSpace& DataSpace() const = 0;
  virtual void Read(const void* dataOut_) const = 0;
};
