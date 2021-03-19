#pragma once
#include <string>


class IDataSet
{
public:
  virtual ~IDataSet() = default;
  
  virtual const std::string& Name() const = 0;
};
