#pragma once
#include <string>
#include <hdf5/H5Cpp.h>
#include "datasets/DatasetProperties.hpp"
#include "datasets/DatasetDefinition.h"


class AscanDataset
{
protected:
  AscanDataset(hid_t Id_, const std::string& location_)
    : m_dsetId(Id_)
    , m_location(location_)
  {
    m_dspaceId = H5Dget_space(m_dsetId);
    const int ndims = H5Sget_simple_extent_ndims(m_dspaceId);

    hsize_t* dsetDims = new hsize_t[ndims]{};
    H5Sget_simple_extent_dims(m_dspaceId, dsetDims, nullptr);

    if (ndims == 2) {
      m_dims = DataDimensions(dsetDims[0], dsetDims[1]);
    }
    else if (ndims == 3) {
      m_dims = DataDimensions(dsetDims[0], dsetDims[1], dsetDims[2]);
    }
    
    delete[] dsetDims;

    hsize_t dimsm[1] = { m_dims.Z };
    m_singleDspaceId = H5Screate_simple(1, dimsm, nullptr);

    hid_t plistId = H5Dget_create_plist(m_dsetId);

    if (H5Pget_layout(plistId) == H5D_CHUNKED)
    {
      hsize_t* chunkDims = new hsize_t[ndims]{};
      int rankChunk = H5Pget_chunk(plistId, ndims, chunkDims);
      m_properties = DatasetProperties(true, DataDimensions(chunkDims[0], chunkDims[1], chunkDims[2]));
      delete[] chunkDims;

      int numfilt = H5Pget_nfilters(plistId);

      for (int i{}; i < numfilt; i++)
      {
        size_t nelmts{};
        unsigned int flags{};
        unsigned int filterInfo{};

        H5Z_filter_t filterType = H5Pget_filter(plistId, 0, &flags, &nelmts, nullptr, 0, nullptr, &filterInfo);

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

        filterType = H5Z_FILTER_NONE;
      }
    }
  }

  AscanDataset() = delete;
  AscanDataset(const AscanDataset&) = delete;
  AscanDataset& operator=(const AscanDataset&) = delete;

  virtual ~AscanDataset()
  {
    H5Dclose(m_dspaceId);
    H5Dclose(m_dsetId);
  }

  const std::string& Location() const {
    return m_location;
  };

  const AscanAttributes& Attributes() const {
    return m_attributes;
  }

  const DataDimensions& Dimensions() const {
    return m_dims;
  }

  const DatasetProperties& Properties() const {
    return m_properties;
  };

  void SelectSingle(size_t x_, size_t y_) const
  {
    hsize_t count[3] = { 1, 1, m_dims.Z };
    hsize_t offset[3] = { x_, y_, 0 };
    H5_RESULT_CHECK(H5Sselect_hyperslab(m_dspaceId, H5S_SELECT_SET, offset, nullptr, count, nullptr));
  };

  void Read(void* dataOut_) const
  {
    H5_RESULT_CHECK(H5Dread(m_dsetId, H5T_NATIVE_CHAR, m_singleDspaceId, m_dspaceId, H5P_DEFAULT, dataOut_));
  }
  

private:
  hid_t m_dsetId{};
  hid_t m_dspaceId{};
  hid_t m_singleDspaceId{};
  DataDimensions m_dims;
  DatasetProperties m_properties;
  AscanAttributes m_attributes;
  const std::string m_location;
};
