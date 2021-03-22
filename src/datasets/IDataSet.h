#pragma once
#include <string>


class IDataset
{
public:
  virtual ~IDataset() = default;
  
  virtual const std::string& Location() const = 0;
};
