#pragma once
#include "IDataSet.h"


struct AscanAttributes;
class AscanDataSpace;

class IAscanDataSet : public IDataSet
{
public:
  virtual ~IAscanDataSet() = default;
  
  virtual const AscanAttributes& Attributes() const = 0;
  virtual const AscanDataSpace& DataSpace() const = 0;
  virtual void Read(const void* dataOut_) const = 0;
};
