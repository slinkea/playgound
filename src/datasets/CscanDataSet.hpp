#pragma once
#include "DataSet.hpp"
#include "CscanDataSpace.hpp"


class CscanDataSet : public DataSet
{
protected:
  CscanDataSet(hid_t id_, const std::wstring& configName_, const std::wstring& sourceName_)
    : DataSet(id_, configName_)
    , m_sourceName(sourceName_)
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

  CscanDataSpace m_dataSpace;
  CscanAttributes m_attributes;
  const std::wstring m_sourceName;
};
