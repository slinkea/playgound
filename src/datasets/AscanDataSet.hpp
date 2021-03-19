#pragma once
#include "DataSet.hpp"
#include "AscanDataSpace.hpp"


constexpr char ASCAN_DATASET[] = "Ascan Data";
constexpr char ASCAN_STATUS_DATASET[] = "Ascan Status";

class AscanDataSet : public DataSet
{
protected:
  AscanDataSet(hid_t id_, const std::wstring& configName_)
    : DataSet(configName_)
  {
    m_id = H5Dopen(id_, ASCAN_DATASET, H5P_DEFAULT);
    m_id = H5Dopen(id_, ASCAN_STATUS_DATASET, H5P_DEFAULT);

    hid_t dspaceId = H5Dget_space(m_id);
    const int ndims = H5Sget_simple_extent_ndims(dspaceId);

    hsize_t* dsetDims = new hsize_t[ndims]{};
    H5Sget_simple_extent_dims(dspaceId, dsetDims, nullptr);

    DataDimensions dataDimensions(dsetDims[0], dsetDims[1], dsetDims[2]);
    m_dataSpace = AscanDataSpace(dataDimensions);

    delete[] dsetDims;

    hid_t propLists = H5Dget_create_plist(m_id);

    if (H5Pget_layout(propLists) == H5D_CHUNKED)
    {
      hsize_t* chunkDims = new hsize_t[ndims]{};
      int rankChunk = H5Pget_chunk(propLists, ndims, chunkDims);
      delete[] chunkDims;

      size_t nelmts{};
      unsigned int flags{};
      unsigned int filterInfo{};
        
      H5Z_filter_t filterType = H5Pget_filter(propLists, 0, &flags, &nelmts, nullptr, 0, nullptr, &filterInfo);

      switch (filterType) {
        case H5Z_FILTER_DEFLATE:
          break;
        case H5Z_FILTER_SHUFFLE:
          break;
        case H5Z_FILTER_FLETCHER32:
          break;
        case H5Z_FILTER_SZIP:
          break;
        case H5Z_FILTER_NBIT:
          break;
        case H5Z_FILTER_SCALEOFFSET:
          break;
      }

      nelmts = 0;
    }
  }

  AscanDataSet() = delete;
  AscanDataSet(const AscanDataSet&) = delete;
  AscanDataSet& operator=(const AscanDataSet&) = delete;

  virtual ~AscanDataSet() = default;

  const AscanAttributes& Attributes() const {
    return m_attributes;
  }

  const AscanDataSpace& DataSpace() const {
    return m_dataSpace;
  };

private:
  AscanAttributes m_attributes;
  AscanDataSpace m_dataSpace;
};
