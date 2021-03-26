#pragma once
#include "Container.hpp"
#include "DataSource.hpp"
#include "IDataset.h"


class IData
{
public:
  virtual ~IData() = default;

  virtual const DataSource& Source() const = 0;
  virtual const ONDTLib::Container<IDataset>& Datasets() const = 0;
  virtual ONDTLib::Container<IDataset>& Datasets() = 0;
};

using TIDataPtrs = std::vector<IData*>;
