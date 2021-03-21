#pragma once
#include <string>
#include "CscanDataspace.hpp"


class CscanDataset
{
protected:
  CscanDataset(hid_t Id, const std::wstring& configName_, const std::wstring& sourceName_)
    : m_dataId(Id)
    , m_configName(configName_)
    , m_sourceName(sourceName_)
  {
  }

  CscanDataset() = delete;
  CscanDataset(const CscanDataset&) = delete;
  CscanDataset& operator=(const CscanDataset&) = delete;

  virtual ~CscanDataset() = default;

  const std::wstring& Configuration() const {
    return m_configName;
  };

  const CscanAttributes& Attributes() const {
    return m_attributes;
  }

  const CscanDataspace& Dataspace() const {
    return m_dataspace;
  };

  void Fetch()
  {

  }

  hid_t m_dataId{};
  CscanDataspace m_dataspace;
  CscanAttributes m_attributes;
  const std::wstring m_sourceName;
  const std::wstring m_configName;
};
