#pragma once
#include "Dataset.hpp"
#include "AscanDataspace.hpp"


class AscanDataset : public Dataset
{
protected:
  AscanDataset(hid_t id_, const std::wstring& configName_)
    : Dataset(id_, configName_)
  {
    hid_t dspaceId = H5Dget_space(m_id);
    const int ndims = H5Sget_simple_extent_ndims(dspaceId);

    hsize_t* dsetDims = new hsize_t[ndims]{};
    H5Sget_simple_extent_dims(dspaceId, dsetDims, nullptr);

    DataDimensions dataDimensions(dsetDims[0], dsetDims[1], dsetDims[2]);
    m_dataspace = AscanDataspace(dataDimensions);

    delete[] dsetDims;

    hid_t plistId = H5Dget_create_plist(m_id);

    if (H5Pget_layout(plistId) == H5D_CHUNKED)
    {
      hsize_t* chunkDims = new hsize_t[ndims]{};
      int rankChunk = H5Pget_chunk(plistId, ndims, chunkDims);
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

  virtual ~AscanDataset() = default;

  const AscanAttributes& Attributes() const {
    return m_attributes;
  }

  const AscanDataspace& Dataspace() const {
    return m_dataspace;
  };

private:
  AscanAttributes m_attributes;
  AscanDataspace m_dataspace;
};
