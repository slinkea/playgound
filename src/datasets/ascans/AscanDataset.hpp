#pragma once
#include "datasets/Dataset.hpp"
#include "datasets/DatasetDefinition.h"


class AscanDataset : public Dataset
{
public:
  AscanDataset(hid_t dsetId_, const std::string& location_)
    : Dataset(dsetId_, location_)
  {
    m_offset = CreateArray(m_dimQty);
    m_count = CreateArray(m_dimQty);
    m_count[0] = 1;
    m_count[1] = 1;
    m_count[2] = m_dataDims.Z;

    m_pointQty[0] = m_dataDims.Z;
    m_singleId = H5Screate_simple(1, m_pointQty, nullptr);

    m_sampleSize = H5Tget_size(m_dataType);
    m_sampleType = GetDataType();
  }

  AscanDataset(const AscanDataset&) = default;
  virtual ~AscanDataset() = default;

  AscanDataset() = delete;
  AscanDataset& operator=(const AscanDataset&) = delete;

  const AscanAttributes& Attributes() const {
    return m_attributes;
  }

  DataType SampleType() const {
    return m_sampleType;
  }

  size_t SampleSize() const {
    return m_sampleSize;
  }

  void SelectSingle(size_t x_, size_t y_) const
  {
    m_offset[0] = x_;
    m_offset[1] = y_;

    H5_RESULT_CHECK(H5Sselect_hyperslab(m_dspaceId, H5S_SELECT_SET, m_offset, nullptr, m_count, nullptr));
  };

  void Read(void* dataOut_) const
  {
    H5_RESULT_CHECK(H5Dread(m_dsetId, m_dataType, m_singleId, m_dspaceId, H5P_DEFAULT, dataOut_));
  }

private:
  hid_t m_singleId{};
  DataType m_sampleType;
  size_t m_sampleSize{};
  hsize_t m_pointQty[1]{};
  AscanAttributes m_attributes;
};
