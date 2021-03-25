#pragma once
#include "DatasetDefinition.h"


class DatasetProperties
{
public:
  DatasetProperties(bool chunked_, bool compressed_, const DataDimensions& dims_)
    : m_chunked(chunked_)
    , m_compressed(compressed_)
    , m_dims(dims_)
  {
  }

  DatasetProperties() = default;
  DatasetProperties(const DatasetProperties&) = delete;

  virtual ~DatasetProperties() = default;

  bool IsChunked() const {
    return m_chunked;
  }

  bool IsCompressed() const {
    return m_compressed;
  }

  const DataDimensions& ChunkDimensions() const {
    return m_dims;
  }

private:
  bool m_chunked{};
  bool m_compressed{};
  DataDimensions m_dims;
};
