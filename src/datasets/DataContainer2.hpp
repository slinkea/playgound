#pragma once
#include "IData2.h"
#include "Container.hpp"
#include "DatasetContainer2.hpp"


class DataContainer2 : public ONDTLib::Container<IData2>
{
  using TSuper = ONDTLib::Container<IData2>;

public:
  DataContainer2() = default;
  virtual ~DataContainer2() = default;

  DataContainer2(const DataContainer2&) = delete;
  DataContainer2& operator=(const DataContainer2&) = delete;

  DataContainer2(DataContainer2&& other_) noexcept
    : Container<IData2>(std::move(other_))
    //, m_datasets(std::move(other_.m_datasets))
  {
  }

  DataContainer2& operator=(DataContainer2&& other_) noexcept
  {
    if (this != &other_)
    {
      Container<IData2>::operator=(std::move(other_));
      //m_datasets = std::move(other_.m_datasets);
    }

    return *this;
  }

  //const DatasetContainer2& Datasets() const {
  //  return m_items;
  //}

  //DatasetContainer2& Datasets() {
  //  return m_items;
  //}

private:
  //DatasetContainer2 m_datasets;
};
