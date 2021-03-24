#pragma once
#include <vector>
#include "IAscanData.h"


class IAscanDataVector : public std::vector<const IAscanData*>
{
public:
  IAscanDataVector() = default;
  IAscanDataVector& operator=(const IAscanDataVector&) = delete;

  virtual ~IAscanDataVector() = default;

  const IAscanData* Get(size_t configId_) const
  {
    auto itr = std::find_if(begin(), end(), 
      [&configId_](const IAscanData* a) { return a->Source()->Id() == configId_; });
    return itr != end() ? (*itr) : nullptr;
  }

private:

};
