#pragma once
#include "Container.hpp"
#include "CscanDataset.hpp"
#include "CscanBeamDataset.hpp"


class CscanDatasetContainer : public ONDTLib::Container<Dataset>
{
  using TSuper = ONDTLib::Container<Dataset>;

public:
  CscanDatasetContainer() = default;
  virtual ~CscanDatasetContainer() = default;

  CscanDatasetContainer(const CscanDatasetContainer&) = delete;
  CscanDatasetContainer& operator=(const CscanDatasetContainer&) = delete;

  CscanDatasetContainer(CscanDatasetContainer&& other_) noexcept
    : ONDTLib::Container<Dataset>(std::move(other_))
  {
  }

  CscanDatasetContainer& operator=(CscanDatasetContainer&& other_) noexcept
  {
    if (this != &other_)
    {
      ONDTLib::Container<Dataset>::operator=(std::move(other_));
    }

    return *this;
  }

  CscanDataset* Find(size_t gateId_) const
  {
    return dynamic_cast<CscanDataset*>(TSuper::Find(
      [&gateId_](const TItemPtr& item_)
      {
        if (dynamic_cast<CscanBeamDataset*>(item_.get())) {
          return false;
        }
        else 
        {
          const auto& map = dynamic_cast<CscanDataset*>(item_.get())->GateIdentifier();
          return map.find(gateId_) != map.end();
        }
      }
    ));
  }

  const CscanBeamDataset* Find(size_t gateId_, size_t beamIdx_) const
  {
    return dynamic_cast<const CscanBeamDataset*>(TSuper::Find(
      [&](const TItemPtr& item_)
      {
        if (const auto dset = dynamic_cast<const CscanBeamDataset*>(item_.get()))
        {
          if (dset->BeamIndex() == beamIdx_) 
          {
            const auto& map = dset->GateIdentifier();
            return map.find(gateId_) != map.end();
          }
          else {
            return false;
          }
        }
        else {
          return false;
        }
      }
    ));
  }
};
