#pragma once


class IReadOnlyData
{
public:
  virtual ~IReadOnlyData() = default;

  virtual void Read(void* dataOut_) const = 0;

};
