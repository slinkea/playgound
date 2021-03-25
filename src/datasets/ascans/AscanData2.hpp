#pragma once
#include "AscanDataSource2.hpp"


class AscanData2 : public IData2
{
public:
  AscanData2(const AscanDataSource2& dataSource_)
    : m_dataSource(dataSource_)
  {
  }

  AscanData2() = delete;
  AscanData2(const AscanData2&) = delete;
  AscanData2& operator=(const AscanData2&) = delete;

  virtual ~AscanData2() = default;

  const AscanDataSource2& Source() const override {
    return m_dataSource;
  }



private:
  const AscanDataSource2 m_dataSource;
};
