#pragma once
#include "DataAccess.h"


class AscanDataSpace
{
public:
  AscanDataSpace() = default;
  AscanDataSpace(const DataDimensions& dataDimensions_)
    : m_dataDimensions(dataDimensions_)
  {
  }

  AscanDataSpace(const AscanDataSpace&) = delete;
  AscanDataSpace& operator=(const AscanDataSpace&) = delete;
  virtual ~AscanDataSpace() = default;

  const DataDimensions& Dimensions() const {
    return m_dataDimensions;
  }

private:
  DataDimensions m_dataDimensions;
};
