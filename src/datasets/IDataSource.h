#pragma once
#include <string>


class IDataSource
{
public:
  virtual ~IDataSource() = default;

  virtual const std::wstring& Name() const = 0;

};
