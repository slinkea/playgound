#pragma once
#include "datasets/Dataset2.hpp"


class AscanDataset2 : public Dataset2
{
public:
  AscanDataset2(hid_t Id_, const std::string& location_)
    : Dataset2(Id_, location_)
  {
    m_offset = new hsize_t[m_dimQty]{};
    m_countSel = new hsize_t[m_dimQty]{};

    m_pointQty[0] = m_dataDims.Z;
    m_singleId = H5Screate_simple(1, m_pointQty, nullptr);
  }

  AscanDataset2(const AscanDataset2&) = default;
  virtual ~AscanDataset2() = default;

  AscanDataset2() = delete;
  AscanDataset2& operator=(const AscanDataset2&) = delete;

  const AscanAttributes& Attributes() const {
    return m_attributes;
  }

  void SelectSingle(size_t x_, size_t y_) const
  {
    m_offset[0] = x_;
    m_offset[1] = y_;

    H5_RESULT_CHECK(H5Sselect_hyperslab(m_dspaceId, H5S_SELECT_SET, m_offset, nullptr, m_countSel, nullptr));
  };

  void Read(void* dataOut_) const
  {
    H5_RESULT_CHECK(H5Dread(m_dsetId, H5T_NATIVE_CHAR, m_singleId, m_dspaceId, H5P_DEFAULT, dataOut_));
  }

private:
  hid_t m_singleId{};
  hsize_t m_pointQty[1]{};
  AscanAttributes m_attributes;
};
