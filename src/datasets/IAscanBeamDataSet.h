#pragma once
#include <memory>
#include <vector>
#include "IDataSet.h"


class IAscanBeamDataSet : public IDataSet
{
public:
  virtual ~IAscanBeamDataSet() = default;
  
  virtual size_t BeamIndex() const = 0;
  virtual const AscanAttributes& Attributes() const = 0;
  virtual const AscanDataSpace& DataSpace() const = 0;
  virtual void Read(const void* dataOut_) const = 0;
};

using TIAscanBeamDataSets = std::vector<IAscanBeamDataSet*>;
using TIAscanBeamDataSetPtr = std::unique_ptr<IAscanBeamDataSet>;
