#pragma once
#include <vector>
#include "IAscanDataset.h"


class IAscanDatasetVector : public std::vector<TIAscanDatasetPtr>
{
public:
  IAscanDatasetVector() = default;
  IAscanDatasetVector& operator=(const IAscanDatasetVector&) = delete;

  virtual ~IAscanDatasetVector() = default;

  TIAscanDatasetPtr Filter(size_t beamIdx_) const
  {
    auto itr = std::find_if(begin(), end(),
      [&beamIdx_](TIAscanDatasetPtr a) { return true; /* a-> == beamIdx_;*/ });
    return itr != end() ? (*itr) : nullptr;
  }

private:

};
