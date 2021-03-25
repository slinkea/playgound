#pragma once
#include "Container.hpp"
#include "Dataset2.hpp"


class DatasetContainer2 : public ONDTLib::Container<Dataset2>
{
  using TSuper = ONDTLib::Container<Dataset2>;

public:
  DatasetContainer2() = default;
  virtual ~DatasetContainer2() = default;

  DatasetContainer2(const DatasetContainer2&) = delete;
  DatasetContainer2& operator=(const DatasetContainer2&) = delete;

  DatasetContainer2(DatasetContainer2&& other_) noexcept
    : Container<Dataset2>(std::move(other_))
  {
  }

  DatasetContainer2& operator=(DatasetContainer2&& other_) noexcept
  {
    if (this != &other_)
    {
      Container<Dataset2>::operator=(std::move(other_));
    }

    return *this;
  }
};
