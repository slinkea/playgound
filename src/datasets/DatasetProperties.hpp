#pragma once
#include "DatasetDefinition.h"


class DatasetProperties
{
public:
  DatasetProperties(bool chunked_, const DataDimensions& dims_)
    : m_chunked(chunked_)
    , m_dims(dims_)
  {
  }

  DatasetProperties() = default;
  DatasetProperties(const DatasetProperties&) = delete;

  virtual ~DatasetProperties() = default;

  bool IsChunked() const {
    return m_chunked;
  }

  const DataDimensions& ChunkDimensions() const {
    return m_dims;
  }

private:
  bool m_chunked{};
  DataDimensions m_dims;
};
