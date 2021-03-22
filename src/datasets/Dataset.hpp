#pragma once
#include <hdf5/H5Cpp.h>


class Dataset
{
protected:
  Dataset(hid_t id_)
    : m_id(id_)
  {
  }

  Dataset() = delete;
  Dataset(const Dataset&) = delete;
  Dataset& operator=(const Dataset&) = delete;

  virtual ~Dataset() = default;


  hid_t m_id{};
};
