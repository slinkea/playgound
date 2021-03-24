#pragma once


class IData
{
public:
  virtual ~IData() = default;

  virtual void Read(void* dataOut_) const = 0;

};
