#pragma once
#include "IDataSet.h"


struct CscanAttributes;
class CscanDataSpace;

class ICscanDataSet : public IDataSet
{
public:
  virtual ~ICscanDataSet() = default;
  
  virtual const CscanAttributes& Attributes() const = 0;
  virtual const CscanDataSpace& DataSpace() const = 0;
  virtual void Read(const void* dataOut_) const = 0;
};
