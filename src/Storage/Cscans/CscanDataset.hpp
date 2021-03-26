#pragma once
#include "Storage/Dataset.hpp"
#include "Storage/DatasetDefinition.h"


class CscanDataset : public Dataset
{
public:
  CscanDataset(hid_t dsetId_, const std::string& location_, const TGateIdentifiers& gateIds_)
    : Dataset(dsetId_, location_)
    , m_gateIds(gateIds_)
  {
  }

  CscanDataset(const CscanDataset&) = default;
  virtual ~CscanDataset() = default;

  CscanDataset() = delete;
  CscanDataset& operator=(const CscanDataset&) = delete;

  const CscanAttributes& Attributes() const {
    return m_attributes;
  }

  const TGateIdentifiers& GateIdentifier() const {
    return m_gateIds;
  }

  void SelectAll() const
  {
    //m_offset[0] = x_;
    //m_offset[1] = y_;

    H5_RESULT_CHECK(H5Sselect_hyperslab(m_dspaceId, H5S_SELECT_SET, m_offset, nullptr, m_count, nullptr));
  };

  void Read(void* dataOut_) const
  {
    H5_RESULT_CHECK(H5Dread(m_dsetId, m_dataType, m_singleId, m_dspaceId, H5P_DEFAULT, dataOut_));
  }

private:
  hid_t m_singleId{};
  CscanAttributes m_attributes;
  const TGateIdentifiers m_gateIds;
};
