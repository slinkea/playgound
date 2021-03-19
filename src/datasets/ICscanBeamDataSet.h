#pragma once
#include "IDataSet.h"


struct CscanAttributes;
class CscanDataSpace;

class ICscanBeamDataSet : public IDataSet
{
public:
  virtual ~ICscanBeamDataSet() = default;
  
  virtual size_t BeamIndex() const = 0;
  virtual const CscanAttributes& Attributes() const = 0;
  virtual const CscanDataSpace& DataSpace() const = 0;
  virtual void Read(const void* dataOut_) const = 0;
};
