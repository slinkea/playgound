#pragma once
#include "Container.hpp"
#include "Dataset.hpp"


class DatasetContainer : public ONDTLib::Container<Dataset>
{
  using TSuper = ONDTLib::Container<Dataset>;

public:
  DatasetContainer() = default;
  virtual ~DatasetContainer() = default;

  DatasetContainer(const DatasetContainer&) = delete;
  DatasetContainer& operator=(const DatasetContainer&) = delete;

  DatasetContainer(DatasetContainer&& other_) noexcept
    : Container<Dataset>(std::move(other_))
  {
  }

  DatasetContainer& operator=(DatasetContainer&& other_) noexcept
  {
    if (this != &other_)
    {
      Container<Dataset>::operator=(std::move(other_));
    }

    return *this;
  }
};
