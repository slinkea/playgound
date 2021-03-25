#pragma once
#include "Container.hpp"
#include "IData2.h"


class DataContainer2 : public ONDTLib::Container<IData2>
{
  using TSuper = ONDTLib::Container<IData2>;

public:
  DataContainer2()
  {
  }

  DataContainer2(const DataContainer2&) = delete;
  DataContainer2& operator=(const DataContainer2&) = delete;
  virtual ~DataContainer2() {}

  DataContainer2(DataContainer2&& other_) noexcept
    : Container<IData2>(std::move(other_))
  {
  }

  DataContainer2& operator=(DataContainer2&& other_) noexcept
  {
    if (this != &other_)
    {
      Container<IData2>::operator=(std::move(other_));
    }

    return *this;
  }

private:

};
