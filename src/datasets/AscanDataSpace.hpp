#pragma once
#include "DataAccess.h"


class AscanDataspace
{
public:
  AscanDataspace() = default;
  AscanDataspace(const DataDimensions& dimensions_)
    : m_dimensions(dimensions_)
  {
  }

  virtual ~AscanDataspace() = default;

  const DataDimensions& Dimensions() const {
    return m_dimensions;
  }

private:
  DataDimensions m_dimensions;
};
