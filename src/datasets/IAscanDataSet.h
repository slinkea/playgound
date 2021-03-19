#pragma once
#include <memory>
#include <vector>
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

using TIAscanDataSets = std::vector<IAscanDataSet*>;
using TIAscanDataSetPtr = std::unique_ptr<IAscanDataSet>;
