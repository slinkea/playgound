#pragma once
#include "Container.hpp"
#include "AscanDataset.hpp"
#include "AscanBeamDataset.hpp"


class AscanDatasetContainer : public ONDTLib::Container<DatasetBase>
{
  using TSuper = ONDTLib::Container<DatasetBase>;

public:
  AscanDatasetContainer() = default;
  virtual ~AscanDatasetContainer() = default;

  AscanDatasetContainer(const AscanDatasetContainer&) = delete;
  AscanDatasetContainer& operator=(const AscanDatasetContainer&) = delete;

  AscanDatasetContainer(AscanDatasetContainer&& other_) noexcept
    : ONDTLib::Container<DatasetBase>(std::move(other_))
  {
  }

  AscanDatasetContainer& operator=(AscanDatasetContainer&& other_) noexcept
  {
    if (this != &other_)
    {
      ONDTLib::Container<DatasetBase>::operator=(std::move(other_));
    }

    return *this;
  }

  AscanDataset* Dataset() const
  {
    return dynamic_cast<AscanDataset*>(TSuper::Find(
      [](const TItemPtr& item_) { return dynamic_cast<AscanDataset*>(item_.get()); }
    ));
  }

  const AscanBeamDataset* Dataset(size_t beamIdx_) const
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
