#pragma once
#include "Container.hpp"
#include "AscanDataset.hpp"
#include "AscanDataset.hpp"
#include "AscanStatusDataset.hpp"


class AscanDatasetContainer : public ONDTLib::Container<Dataset>
{
  using TSuper = ONDTLib::Container<Dataset>;

public:
  AscanDatasetContainer() = default;
  virtual ~AscanDatasetContainer() = default;

  AscanDatasetContainer(const AscanDatasetContainer&) = delete;
  AscanDatasetContainer& operator=(const AscanDatasetContainer&) = delete;

  AscanDatasetContainer(AscanDatasetContainer&& other_) noexcept
    : ONDTLib::Container<Dataset>(std::move(other_))
  {
  }

  AscanDatasetContainer& operator=(AscanDatasetContainer&& other_) noexcept
  {
    if (this != &other_)
    {
      ONDTLib::Container<Dataset>::operator=(std::move(other_));
    }

    return *this;
  }

  AscanDataset* Data() const
  {
    return dynamic_cast<AscanDataset*>(TSuper::Find(
      [](const TItemPtr& item_) { return dynamic_cast<AscanDataset*>(item_.get()); }
    ));
  }

  AscanStatusDataset* Status() const
  {
    return dynamic_cast<AscanStatusDataset*>(TSuper::Find(
      [](const TItemPtr& item_) { return dynamic_cast<AscanStatusDataset*>(item_.get()); }
    ));
  }
};
