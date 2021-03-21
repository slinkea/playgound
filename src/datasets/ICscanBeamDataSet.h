#pragma once
#include "IDataset.h"


struct CscanAttributes;
class CscanDataspace;

class ICscanBeamDataset : public IDataset
{
public:
  virtual ~ICscanBeamDataset() = default;
  
  virtual size_t BeamIndex() const = 0;
  virtual const CscanAttributes& Attributes() const = 0;
  virtual const CscanDataspace& Dataspace() const = 0;
  virtual void Read(const void* dataOut_) const = 0;
};
