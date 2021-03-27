#pragma once
#include "Container.hpp"
#include "AscanDataset.hpp"
#include "AscanBeamDataset.hpp"


class AscanDatasetContainer : public ONDTLib::Container<AscanDataset>
{
  using TSuper = ONDTLib::Container<AscanDataset>;

public:
  AscanDatasetContainer() = default;
  virtual ~AscanDatasetContainer() = default;

  AscanDatasetContainer(const AscanDatasetContainer&) = delete;
  AscanDatasetContainer& operator=(const AscanDatasetContainer&) = delete;

  AscanDatasetContainer(AscanDatasetContainer&& other_) noexcept
    : ONDTLib::Container<AscanDataset>(std::move(other_))
  {
  }

  AscanDatasetContainer& operator=(AscanDatasetContainer&& other_) noexcept
  {
    if (this != &other_)
    {
      ONDTLib::Container<AscanDataset>::operator=(std::move(other_));
    }

    return *this;
  }

  const TItemPtrs& Items() const {
    return TSuper::Items();
  }

  const AscanDataset* Dataset() const
  {
    return dynamic_cast<const AscanDataset*>(TSuper::Find(
      [](const TItemPtr& item_) { return dynamic_cast<const AscanDataset*>(item_.get()); }
    ));
  }

  const AscanBeamDataset* BeamDataset(size_t beamIdx_) const
  {
    return dynamic_cast<const AscanBeamDataset*>(TSuper::Find(
      [&beamIdx_](const TItemPtr& item_)
      {
        if (const auto dset = dynamic_cast<const AscanBeamDataset*>(item_.get())) {
          return dset->BeamIndex() == beamIdx_;
        }
        else {
          return false;
        }
      }
    ));
  }
};
