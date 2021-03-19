#pragma once
#include "DataSet.hpp"
#include "CscanDataSpace.hpp"


class CscanDataSet : public DataSet
{
protected:
  CscanDataSet(hid_t id_)
  : DataSet(id_, L"")
  {
  }

  CscanDataSet() = delete;
  CscanDataSet(const CscanDataSet&) = delete;
  CscanDataSet& operator=(const CscanDataSet&) = delete;

  virtual ~CscanDataSet() = default;

  const CscanAttributes& Attributes() const {
    return m_attributes;
  }

  const CscanDataSpace& DataSpace() const {
    return m_dataSpace;
  };

  void Fetch()
  {

  }

  CscanAttributes m_attributes;
  CscanDataSpace m_dataSpace;
};
