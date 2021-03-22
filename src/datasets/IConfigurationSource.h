#pragma once
#include <string>


class IConfigurationSource
{
public:
  virtual ~IConfigurationSource() = default;

  virtual const std::wstring& Name() const = 0;
};
