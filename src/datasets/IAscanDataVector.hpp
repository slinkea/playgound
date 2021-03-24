#pragma once
#include <vector>
#include "IAscanData.h"


class IAscanDataVector : public std::vector<const IAscanData*>
{
public:
  IAscanDataVector() = default;
  IAscanDataVector& operator=(const IAscanDataVector&) = delete;

  virtual ~IAscanDataVector() = default;

  const IAscanData* Filter(size_t configId_) const
  {
    auto itr = std::find_if(begin(), end(), 
      [&configId_](const IAscanData* a) { return a->Source()->Id() == configId_; });
    return itr != end() ? (*itr) : nullptr;
  }

  const IAscanDataVector Filter(const std::wstring& configName_) const
  {
    IAscanDataVector ascanDataVec;
    std::copy_if(begin(), end(), std::back_inserter(ascanDataVec),
      [&configName_](const IAscanData* a) { return a->Source()->Name() == configName_; });

    return ascanDataVec;
  }

private:

};
