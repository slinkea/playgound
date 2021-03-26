#pragma once
#include "Container.hpp"
#include "DataSource.hpp"
#include "DatasetBase.hpp"


class IData
{
public:
  virtual ~IData() = default;

  virtual const DataSource& Source() const = 0;
  virtual const ONDTLib::Container<DatasetBase>& Datasets() const = 0;
  virtual ONDTLib::Container<DatasetBase>& Datasets() = 0;
};

using TIDataPtrs = std::vector<IData*>;
