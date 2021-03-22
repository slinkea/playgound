#pragma once
#include <string>
#include <hdf5/H5Cpp.h>
#include "AscanDataspace.hpp"


class AscanDataDataset
{
protected:
  AscanDataDataset(hid_t Id_)
    : m_dataId(Id_)
  {
  }

  AscanDataDataset() = delete;
  AscanDataDataset(const AscanDataDataset&) = delete;
  AscanDataDataset& operator=(const AscanDataDataset&) = delete;

  virtual ~AscanDataDataset() = default;

private:
  hid_t m_dataId{};
  hid_t m_statusId{};
  AscanDataspace m_dataspace;
  AscanAttributes m_attributes;
  const std::wstring m_configName;
};
