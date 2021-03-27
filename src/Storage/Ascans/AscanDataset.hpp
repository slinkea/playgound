#pragma once
#include <memory>
#include "Storage/DatasetBase.hpp"
#include "Storage/DatasetDefinition.h"
#include "Storage/DatasetProperties.hpp"


class AscanDataset : public DatasetBase
{
public:
  AscanDataset(hid_t configGroupId_)
  {
    m_dataDsetId = H5Dopen(configGroupId_, ASCAN_DATASET, H5P_DEFAULT);
    //H5Dopen(configGroupId_, ASCAN_STATUS_DATASET, H5P_DEFAULT);

    m_dataType = H5Dget_type(m_dataDsetId);
    m_dataDspaceId = H5Dget_space(m_dataDsetId);
    m_dimQty = H5Sget_simple_extent_ndims(m_dataDspaceId);

    m_dataDims = FetchDataDimensions(m_dataDspaceId, m_dimQty);
    m_properties = FetchProperties(m_dataDsetId, m_dimQty);

    m_offset = CreateArray(m_dimQty);
    m_count = CreateArray(m_dimQty);
    m_count[0] = 1;
    m_count[1] = 1;
    m_count[2] = m_dataDims.SizeZ;

    m_pointQty[0] = m_dataDims.SizeZ;
    m_singleId = H5Screate_simple(1, m_pointQty, nullptr);

    m_sampleSize = H5Tget_size(m_dataType);
    m_sampleType = GetDataType();
  }

  AscanDataset(const AscanDataset&) = default;
  virtual ~AscanDataset()
  {
    delete[] m_offset;
    delete[] m_count;

    H5Dclose(m_dataDsetId);
  }

  AscanDataset() = delete;
  AscanDataset& operator=(const AscanDataset&) = delete;

  const DataDimensions& Dimensions() const {
    return m_dataDims;
  }

  void Read(void* dataOut_) const
  {
    H5_RESULT_CHECK(H5Dread(m_dataDsetId, m_dataType, m_singleId, m_dataDspaceId, H5P_DEFAULT, dataOut_));
  }

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

    H5_RESULT_CHECK(H5Sselect_hyperslab(m_dataDspaceId, H5S_SELECT_SET, m_offset, nullptr, m_count, nullptr));
  };

private:
  DataType GetDataType() const
  {
    size_t size = H5Tget_size(m_dataType);

    switch (H5Tget_class(m_dataType)) {
    case H5T_INTEGER:
      switch (H5Tget_sign(m_dataType)) {
      case H5T_SGN_NONE:
        switch (size) {
        case 1:
          return DataType::UCHAR;
        case 2:
          return DataType::USHORT;
        case 4:
          return DataType::UINT;
        }
      case H5T_SGN_2:
        switch (size) {
        case 1:
          return DataType::CHAR;
        case 2:
          return DataType::SHORT;
        case 4:
          return DataType::INT;
        }
      }
    case H5T_FLOAT:
      switch (size) {
      case 4:
        return DataType::FLOAT;
      case 8:
        return DataType::DOUBLE;
      }
    }

    throw std::runtime_error("Unsupported data type.");
  }

  int m_dimQty{};
  hsize_t* m_offset{};
  hsize_t* m_count{};
  hid_t m_singleId{};
  hid_t m_dataType{};
  hid_t m_dataDsetId{};
  hid_t m_dataDspaceId{};
  DataType m_sampleType;
  size_t m_sampleSize{};
  hsize_t m_pointQty[1]{};
  DataDimensions m_dataDims;
  DataDimensions m_chunkDims;
  AscanAttributes m_attributes;
};

using TAscanDatasetPtr = std::unique_ptr<AscanDataset>;
