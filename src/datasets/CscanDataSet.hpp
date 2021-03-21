#pragma once
#include "Dataset.hpp"
#include "CscanDataspace.hpp"


class CscanDataset : public Dataset
{
protected:
  CscanDataset(hid_t id_, const std::wstring& configName_, const std::wstring& sourceName_)
    : Dataset(id_, configName_)
    , m_sourceName(sourceName_)
  {
  }

  CscanDataset() = delete;
  CscanDataset(const CscanDataset&) = delete;
  CscanDataset& operator=(const CscanDataset&) = delete;

  virtual ~CscanDataset() = default;

  const CscanAttributes& Attributes() const {
    return m_attributes;
  }

  const CscanDataspace& Dataspace() const {
    return m_dataspace;
  };

  void Fetch()
  {

  }

  CscanDataspace m_dataspace;
  CscanAttributes m_attributes;
  const std::wstring m_sourceName;
};
