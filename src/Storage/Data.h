#pragma once
#include "Container.hpp"
#include "DataSource.hpp"
#include "DatasetBase.hpp"


class Data
{
public:
  Data(const DataSource& dataSource_)
    : m_dataSource(dataSource_)
  {
  }

  Data() = delete;
  Data(const Data&) = delete;
  Data& operator=(const Data&) = delete;

  virtual ~Data() = default;

  virtual const DataSource& Source() const {
    return m_dataSource;
  }

private:
  const DataSource m_dataSource;
};

using TIDataPtrs = std::vector<Data*>;
