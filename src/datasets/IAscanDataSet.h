#pragma once
#include "IDataset.h"


struct AscanAttributes;
class AscanDataspace;

class IAscanDataset : public IDataset
{
public:
  virtual ~IAscanDataset() = default;
  
  virtual const AscanAttributes& Attributes() const = 0;
  virtual const AscanDataspace& Dataspace() const = 0;
  virtual void Read(const void* dataOut_) const = 0;
};
