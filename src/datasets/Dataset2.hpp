#pragma once
#include <string>
#include <hdf5/H5Cpp.h>
#include "DatasetDefinition.h"
#include "DatasetProperties.hpp"


class Dataset2
{
public:
  Dataset2(hid_t Id_, const std::string& location_)
    : m_dsetId(Id_)
    , m_location(location_)
  {
  }

  Dataset2() = delete;
  Dataset2(const Dataset2&) = delete;
  Dataset2& operator=(const Dataset2&) = delete;
  virtual ~Dataset2() = default;

  const std::string& Location() const {
    return m_location;
  };

  const DataDimensions& Dimensions() const {
    return m_dims;
  }

  const DatasetProperties& Properties() const {
    return m_properties;
  };

protected:
  std::string m_location;

private:
  hid_t m_dsetId{};
  DataDimensions m_dims;
  DatasetProperties m_properties;
};
