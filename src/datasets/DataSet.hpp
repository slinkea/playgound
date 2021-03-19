#pragma once
#include <string>
#include <hdf5/H5Cpp.h>


class DataSet
{
protected:
  DataSet(hid_t id_)
    : m_id(id_)
  {
  }

  DataSet() = delete;
  DataSet(const DataSet&) = delete;
  DataSet& operator=(const DataSet&) = delete;

  virtual ~DataSet() = default;

  hid_t m_id;
  std::string m_name;
};
