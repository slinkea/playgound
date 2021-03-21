#pragma once
#include "IDataset.h"


struct CscanAttributes;
class CscanDataspace;

class ICscanDataset : public IDataset
{
public:
  virtual ~ICscanDataset() = default;
  
  virtual const CscanAttributes& Attributes() const = 0;
  virtual const CscanDataspace& Dataspace() const = 0;
  virtual void Read(const void* dataOut_) const = 0;
};
