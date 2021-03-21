#pragma once
#include <string>


class IDataset
{
public:
  virtual ~IDataset() = default;
  
  virtual const std::wstring& Configuration() const = 0;
};
