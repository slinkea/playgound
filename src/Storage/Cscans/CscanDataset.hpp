#pragma once
#include <string>
#include "Storage/DatasetBase.hpp"
#include "Storage/DatasetDefinition.h"
#include "Storage/DatasetProperties.hpp"


class CscanDataset : public DatasetBase
{
public:
  CscanDataset(hid_t dsetId_, const TGateIdentifiers& gateIds_)
    : m_dsetId(dsetId_)
    , m_gateIds(gateIds_)
  {
    m_dspaceId = H5Dget_space(m_dsetId);
    m_dataType = H5Dget_type(m_dsetId);

    m_dimQty = H5Sget_simple_extent_ndims(m_dspaceId);
    m_dataDims = FetchDataDimensions(m_dspaceId, m_dimQty);
    m_properties = FetchProperties(m_dsetId, m_dimQty);
    m_count = CreateArray(m_dimQty);
    m_count[0] = m_dataDims.SizeX;
    m_count[1] = m_dataDims.SizeY;

    m_sampleQty[0] = m_dataDims.SizeX;
    m_sampleQty[1] = m_dataDims.SizeY;
    m_selectionId = H5Screate_simple(m_dimQty, m_sampleQty, nullptr);
  }

  CscanDataset(const CscanDataset&) = default;

  virtual ~CscanDataset()
  {
    delete[] m_count;
    H5Dclose(m_dsetId);
  }

  CscanDataset() = delete;
  CscanDataset& operator=(const CscanDataset&) = delete;

  const DataDimensions& Dimensions() const {
    return m_dataDims;
  }

  void Read(void* dataOut_) const
  {
    H5_RESULT_CHECK(H5Dread(m_dsetId, m_dataType, m_selectionId, m_dspaceId, H5P_DEFAULT, dataOut_));
  }

  const CscanAttributes& Attributes() const {
    return m_attributes;
  }

  const TGateIdentifiers& GateIdentifier() const {
    return m_gateIds;
  }

  void SelectAll() const
  {
    H5_RESULT_CHECK(H5Sselect_hyperslab(m_dspaceId, H5S_SELECT_SET, nullptr, nullptr, m_count, nullptr));
  };

private:
  int m_dimQty{};
  hid_t m_dsetId{};
  hid_t m_dspaceId{};
  hid_t m_dataType{};
  hid_t m_selectionId{};
  hsize_t* m_count{};
  hsize_t m_sampleQty[2]{};
  DataDimensions m_dataDims;
  CscanAttributes m_attributes;
  const TGateIdentifiers m_gateIds;
};
