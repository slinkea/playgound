#pragma once
#include "DataAccess.h"


class CscanDataSpace
{
public:
  CscanDataSpace() = default;
  CscanDataSpace(const DataDimensions& dimensions_)
    : m_dimensions(dimensions_)
  {
  }

  CscanDataSpace(const CscanDataSpace&) = delete;
  CscanDataSpace& operator=(const CscanDataSpace&) = delete;
  virtual ~CscanDataSpace() = default;

  const DataDimensions& Dimensions() const {
    return m_dimensions;
  }

private:
  DataDimensions m_dimensions;
};
