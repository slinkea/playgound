#pragma once
#include "AscanBaseDataSet.hpp"
#include "IAscanDataSet.h"


class AscanDataSet : public AscanBaseDataSet, public IAscanDataSet
{
public:
  AscanDataSet(hid_t id_)
  : AscanBaseDataSet(id_)
  {
  }

  AscanDataSet() = delete;
  AscanDataSet(const AscanDataSet&) = delete;
  AscanDataSet& operator=(const AscanDataSet&) = delete;

  virtual ~AscanDataSet() = default;

  const std::string& Name() const override {
    return m_name;
  };

  const AscanAttributes& Attributes() const override {
    return m_attributes;
  }

  const AscanDataSpace& DataSpace() const override {
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
