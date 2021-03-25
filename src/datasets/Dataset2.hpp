#pragma once
#include <string>
#include <hdf5/H5Cpp.h>


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

  virtual const std::string& Location() const = 0;

protected:
  std::string m_location;

private:
  hid_t m_dsetId{};
  
};
