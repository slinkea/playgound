#pragma once
#include "Container.hpp"
#include "AscanDataset.hpp"
#include "AscanDataset.hpp"
#include "AscanBeamDataset.hpp"
#include "AscanStatusDataset.hpp"
#include "AscanStatusBeamDataset.hpp"


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

  const AscanBeamDataset* Data(size_t beamIdx_) const
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

  AscanStatusDataset* Status() const
  {
    return dynamic_cast<AscanStatusDataset*>(TSuper::Find(
      [](const TItemPtr& item_) { return dynamic_cast<AscanStatusDataset*>(item_.get()); }
    ));
  }

  const AscanStatusBeamDataset* Status(size_t beamIdx_) const
  {
    return dynamic_cast<const AscanStatusBeamDataset*>(TSuper::Find(
      [&beamIdx_](const TItemPtr& item_)
      {
        if (const auto dset = dynamic_cast<const AscanStatusBeamDataset*>(item_.get())) {
          return dset->BeamIndex() == beamIdx_;
        }
        else {
          return false;
        }
      }
    ));
  }
};
