#pragma once
#include <string>
#include <hdf5/H5Cpp.h>


class Dataset
{
protected:
  Dataset(hid_t id_, const std::wstring& configName_)
    : m_configName(configName_)
    , m_id(id_)
  {
  }

  Dataset() = delete;
  Dataset(const Dataset&) = delete;
  Dataset& operator=(const Dataset&) = delete;

  virtual ~Dataset() = default;

  const std::wstring& ConfigName() const {
    return m_configName;
  };

  hid_t m_id{};
  const std::wstring m_configName;
};
