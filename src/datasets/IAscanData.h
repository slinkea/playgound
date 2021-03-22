#pragma once
#include "IReadOnlyData.h"


struct AscanAttributes;
class AscanDataspace;

class IAscanData : public IReadOnlyData
{
public:
  virtual ~IAscanData() = default;

  virtual const AscanAttributes& Attributes() const = 0;
  virtual const AscanDataspace& Dataspace() const = 0;
  virtual void Read(const void* dataOut_) const = 0;
};
