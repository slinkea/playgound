#pragma once
#include <string>


class IDataSource
{
public:
  virtual ~IDataSource() = default;

  virtual size_t Id() const = 0;
  virtual const std::wstring& Name() const = 0;

};
