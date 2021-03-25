#pragma once
#include "datasets/Dataset2.hpp"


class AscanDataset2 : public Dataset2
{
public:
  AscanDataset2(hid_t Id_, const std::string& location_)
    : Dataset2(Id_, location_)
  {
  }

  AscanDataset2(const AscanDataset2&) = default;
  virtual ~AscanDataset2() = default;

  AscanDataset2() = delete;
  AscanDataset2& operator=(const AscanDataset2&) = delete;

  const std::string& Location() const override {
    return m_location;
  };

private:

};
