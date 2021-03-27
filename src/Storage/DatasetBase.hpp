#pragma once
#include <hdf5/H5Cpp.h>
#include "H5Utils.hpp"
#include "DatasetDefinition.h"
#include "DatasetProperties.hpp"


class DatasetBase
{
public:
  DatasetBase() = default;
  virtual ~DatasetBase() = default;

  const DatasetProperties& Properties() const {
    return m_properties;
  }

  DataDimensions FetchDataDimensions(hid_t m_dspaceId_, int dimQty_)
  {
    hsize_t* dsetDims = new hsize_t[dimQty_]{};
    H5Sget_simple_extent_dims(m_dspaceId_, dsetDims, nullptr);
    DataDimensions dataDims = CreateDataDimension(dimQty_, dsetDims);
    delete[] dsetDims;

    return dataDims;
  }

  DatasetProperties FetchProperties(hid_t dsetId_, int dimQty_)
  {
    bool chunked{};
    bool compressed{};
    DataDimensions chunkDims;

    hid_t plistId = H5Dget_create_plist(dsetId_);
    if (H5Pget_layout(plistId) == H5D_CHUNKED)
    {
      chunked = true;
      chunkDims = FetchChunkDimensions(plistId, dimQty_);

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
    }

    return DatasetProperties(chunked, compressed, chunkDims);
  }

  hsize_t* CreateArray(size_t dimQty_) {
    return new hsize_t[dimQty_]{};
  }

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

  DataDimensions FetchChunkDimensions(hid_t plistId_, int dimQty_)
  {
    hsize_t* chunkDims = new hsize_t[dimQty_]{};
    int rankChunk = H5Pget_chunk(plistId_, dimQty_, chunkDims);
    DataDimensions chunkDimensions = CreateDataDimension(rankChunk, chunkDims);
    delete[] chunkDims;

    return chunkDimensions;
  }

protected:
  DatasetProperties m_properties;
};
