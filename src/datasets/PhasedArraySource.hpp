#pragma once
#include <string>
#include "IConfigurationSource.h"


class PhasedArraySource : public IConfigurationSource
{
public:
  PhasedArraySource(const std::wstring& name_)
    : m_name(name_)
  {
  }

  PhasedArraySource() = delete;
  PhasedArraySource& operator=(const PhasedArraySource&) = delete;

  virtual ~PhasedArraySource() = default;

  const std::wstring& Name() const override {
    return m_name;
  }

private:
  const std::wstring m_name;
};
