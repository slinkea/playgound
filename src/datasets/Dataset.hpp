#pragma once
#include <string>
#include <hdf5/H5Cpp.h>
#include "DatasetDefinition.h"
#include "DatasetProperties.hpp"


class Dataset
{
public:
  Dataset(hid_t Id_, const std::string& location_)
    : m_dsetId(Id_)
    , m_location(location_)
  {
    m_dspaceId = H5Dget_space(m_dsetId);
    m_dimQty = H5Sget_simple_extent_ndims(m_dspaceId);    

    FetchDataDimensions();
    FetchProperties();
  }

  Dataset() = delete;
  Dataset(const Dataset&) = delete;
  Dataset& operator=(const Dataset&) = delete;

  virtual ~Dataset()
  {
    delete[] m_offset;
    delete[] m_count;
    H5Dclose(m_dspaceId);
    H5Dclose(m_dsetId);
  }

  const std::string& Location() const {
    return m_location;
  };

  const DataDimensions& Dimensions() const {
    return m_dataDims;
  }

  const DatasetProperties& Properties() const {
    return m_properties;
  };

protected:
  hsize_t* CreateDimensionArray(size_t dimQty_) {
    return new hsize_t[dimQty_]{};
  }

  int m_dimQty{};
  hid_t m_dsetId{};
  hid_t m_dspaceId{};
  hsize_t* m_offset{};
  hsize_t* m_count{};
  std::string m_location;
  DataDimensions m_dataDims;

private:
  DataDimensions CreateDataDimension(int dimQty_, const hsize_t* dsetDims_)
  {
    DataDimensions dims;

    if (dimQty_ == 1) {
      dims = DataDimensions(dsetDims_[0]);
    }
    else if (dimQty_ == 2) {
      dims = DataDimensions(dsetDims_[0], dsetDims_[1]);
    }
    else if (dimQty_ == 3) {
      dims = DataDimensions(dsetDims_[0], dsetDims_[1], dsetDims_[2]);
    }

    return dims;
  }

  void FetchDataDimensions()
  {
    hsize_t* dsetDims = new hsize_t[m_dimQty]{};
    H5Sget_simple_extent_dims(m_dspaceId, dsetDims, nullptr);
    m_dataDims = CreateDataDimension(m_dimQty, dsetDims);
    delete[] dsetDims;
  }

  void FetchChunkDimensions(hid_t plistId_)
  {
    hsize_t* chunkDims = new hsize_t[m_dimQty]{};
    int rankChunk = H5Pget_chunk(plistId_, m_dimQty, chunkDims);
    m_chunkDims = CreateDataDimension(rankChunk, chunkDims);
    delete[] chunkDims;
  }

  void FetchProperties()
  {
    bool chunked{};
    hid_t plistId = H5Dget_create_plist(m_dsetId);
    if (H5Pget_layout(plistId) == H5D_CHUNKED)
    {
      chunked = true;
      FetchChunkDimensions(plistId);

      bool compressed{};
      int filterQty = H5Pget_nfilters(plistId);
      for (int filterIdx{}; filterIdx < filterQty; filterIdx++)
      {
        size_t nelmts{};
        unsigned int flags{};
        unsigned int filterInfo{};
        H5Z_filter_t filterType = H5Pget_filter(plistId, 0, &flags, &nelmts, nullptr, 0, nullptr, &filterInfo);
        if (filterType == H5Z_FILTER_DEFLATE) 
        {
          compressed = true;
          break;
        }
      }

      m_properties = DatasetProperties(chunked, compressed, m_chunkDims);
    }
  }

  DataDimensions m_chunkDims;
  DatasetProperties m_properties;
};
