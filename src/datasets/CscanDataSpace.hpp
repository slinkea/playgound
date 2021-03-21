#pragma once
#include "DatasetDefinition.h"


class CscanDataspace
{
public:
  CscanDataspace() = default;
  CscanDataspace(const DataDimensions& dimensions_)
    : m_dimensions(dimensions_)
  {
  }

  CscanDataspace(const CscanDataspace&) = delete;
  CscanDataspace& operator=(const CscanDataspace&) = delete;
  virtual ~CscanDataspace() = default;

  const DataDimensions& Dimensions() const {
    return m_dimensions;
  }

private:
  DataDimensions m_dimensions;
};
