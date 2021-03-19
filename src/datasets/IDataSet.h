#pragma once
#include <string>


class IDataSet
{
public:
  virtual ~IDataSet() = default;
  
  virtual const std::wstring& ConfigName() const = 0;
};
