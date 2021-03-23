#pragma once
#include <string>
#include <hdf5/H5Cpp.h>
#include "AscanDataspace.hpp"


class AscanStatusDataset
{
protected:
  AscanStatusDataset(hid_t Id_, const std::string& location_)
    : m_Id(Id_)
    , m_location(location_)
  {
  
  }

  AscanStatusDataset() = delete;
  AscanStatusDataset(const AscanStatusDataset&) = delete;
  AscanStatusDataset& operator=(const AscanStatusDataset&) = delete;

  virtual ~AscanStatusDataset() = default;

  const std::string& Location() const {
    return m_location;
  };

  const AscanAttributes& Attributes() const {
    return m_attributes;
  }

  const AscanDataspace& Dataspace() const {
    return m_dataspace;
  };

private:
  hid_t m_Id{};
  AscanDataspace m_dataspace;
  AscanAttributes m_attributes;
  const std::string m_location;
};
