#pragma once
#include "Container.hpp"
#include "CscanDataset.hpp"


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

  CscanDataset* Find(const std::wstring& gateName_) const
  {
    return dynamic_cast<CscanDataset*>(TSuper::Find(
      [&gateName_](const TItemPtr& item_) 
      { return true; }
    ));
  }
};
