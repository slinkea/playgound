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

  AscanDataSpace(const AscanDataSpace&) = delete;
  AscanDataSpace& operator=(const AscanDataSpace&) = delete;
  virtual ~AscanDataSpace() = default;

  const DataDimensions& Dimensions() const {
    return m_dimensions;
  }

private:
  DataDimensions m_dimensions;
};
