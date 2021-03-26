#pragma once
#include "Container.hpp"
#include "DataSource.hpp"
#include "Dataset.hpp"


class IData
{
public:
  virtual ~IData() = default;

  virtual const DataSource& Source() const = 0;
  virtual const ONDTLib::Container<Dataset>& Datasets() const = 0;
  virtual ONDTLib::Container<Dataset>& Datasets() = 0;
};
