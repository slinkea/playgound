#pragma once
#include <string>


class IReadOnlyData
{
public:
  virtual ~IReadOnlyData() = default;

  virtual void Read(void* dataOut_) const = 0;

};
