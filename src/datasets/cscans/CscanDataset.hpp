#pragma once
#include <string>
#include "CscanDataspace.hpp"


class CscanDataset
{
protected:
  CscanDataset(hid_t Id, const std::string& location_, const std::wstring& sourceName_)
    : m_dataId(Id)
    , m_location(location_)
    , m_sourceName(sourceName_)
  {
  }

  CscanDataset() = delete;
  CscanDataset(const CscanDataset&) = delete;
  CscanDataset& operator=(const CscanDataset&) = delete;

  virtual ~CscanDataset() = default;

  const std::string& Location() const {
    return m_location;
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
  const std::string m_location;
};
