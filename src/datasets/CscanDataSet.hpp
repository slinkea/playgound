#pragma once
#include "CscanBaseDataSet.hpp"
#include "ICscanDataSet.h"


class CscanDataSet : public CscanBaseDataSet, public ICscanDataSet
{
public:
  CscanDataSet(hid_t id_)
  : CscanBaseDataSet(id_)
  {
  }

  CscanDataSet() = delete;
  CscanDataSet(const CscanDataSet&) = delete;
  CscanDataSet& operator=(const CscanDataSet&) = delete;

  virtual ~CscanDataSet() = default;

  const std::string& Name() const override {
    return m_name;
  };

  const CscanAttributes& Attributes() const override {
    return m_attributes;
  }

  const CscanDataSpace& DataSpace() const override {
    return m_dataSpace;
  };

  void Read(const void* dataOut_) const override
  {
  }

protected:
  void Fetch()
  {

  }
};
