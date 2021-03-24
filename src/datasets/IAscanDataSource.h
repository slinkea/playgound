#pragma once
#include <vector>
#include <memory>
#include "IDataSource.h"

class IDataset;

class IAscanDataSource : public IDataSource
{
public:
  virtual ~IAscanDataSource() = default;
  
  virtual const std::vector<std::shared_ptr<const IDataset>> Datasets() const = 0;
};
