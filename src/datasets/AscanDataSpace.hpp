#pragma once
#include "DataAccess.h"


class AscanDataSpace
{
public:
  AscanDataSpace() = default;
  AscanDataSpace(const DataDimensions& dimensions_)
    : m_dimensions(dimensions_)
  {
  }

  virtual ~AscanDataSpace() = default;

  const DataDimensions& Dimensions() const {
    return m_dimensions;
  }

private:
  DataDimensions m_dimensions;
};
