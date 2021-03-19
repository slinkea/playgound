#pragma once
#include "DataSet.hpp"
#include "CscanDataSpace.hpp"


class CscanBaseDataSet : public DataSet
{
protected:
  CscanBaseDataSet(hid_t id_)
  : DataSet(id_)
  {
  }

  CscanBaseDataSet() = delete;
  CscanBaseDataSet(const CscanBaseDataSet&) = delete;
  CscanBaseDataSet& operator=(const CscanBaseDataSet&) = delete;

  virtual ~CscanBaseDataSet() = default;


  void Fetch()
  {

  }

  CscanAttributes m_attributes;
  CscanDataSpace m_dataSpace;
};
