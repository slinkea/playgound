#pragma once
#include "DataSet.hpp"
#include "AscanDataSpace.hpp"


class AscanDataSet : public DataSet
{
protected:
  AscanDataSet(hid_t id_, const std::wstring& configName_)
  : DataSet(id_, configName_)
  {
  }

  AscanDataSet() = delete;
  AscanDataSet(const AscanDataSet&) = delete;
  AscanDataSet& operator=(const AscanDataSet&) = delete;

  virtual ~AscanDataSet() = default;

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
