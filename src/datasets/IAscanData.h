#pragma once
#include "IReadOnlyData.h"


class IAscanDataSource;
class AscanDataspace;
struct AscanAttributes;

class IAscanData : public IReadOnlyData
{
public:
  virtual ~IAscanData() = default;

  virtual const IAscanDataSource* Source() const = 0;
  virtual const AscanDataspace& Dataspace() const = 0;
  virtual const AscanAttributes& Attributes() const = 0;
};
