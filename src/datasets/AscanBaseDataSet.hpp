#pragma once
#include "DataSet.hpp"
#include "AscanDataSpace.hpp"


class AscanBaseDataSet : public DataSet
{
protected:
  AscanBaseDataSet(hid_t id_)
  : DataSet(id_)
  {
  }

  AscanBaseDataSet() = delete;
  AscanBaseDataSet(const AscanBaseDataSet&) = delete;
  AscanBaseDataSet& operator=(const AscanBaseDataSet&) = delete;

  virtual ~AscanBaseDataSet() = default;

  const AscanAttributes& Attributes() const {
    return m_attributes;
  }

  const AscanDataSpace& DataSpace() const {
    return m_dataSpace;
  };

  void Fetch()
  {

  }

  AscanAttributes m_attributes;
  AscanDataSpace m_dataSpace;
};
