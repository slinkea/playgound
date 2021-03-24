#pragma once
#include <map>
#include <filesystem>
#include "Container.hpp"
#include "IData.h"
#include "ascans/IAscanDataVector.hpp"


class Datasets : public ONDTLib::Container<IData>
{
  using TSuper = ONDTLib::Container<IData>;
public:
  Datasets(const Datasets&) = delete;
  Datasets& operator=(const Datasets&) = delete;
  Datasets() = default;
  virtual ~Datasets() {}

  Datasets(Datasets&& other_) noexcept
    : Container<IData>(std::move(other_))
  {
  }

  Datasets& operator=(Datasets&& other_) noexcept
  {
    if (this != &other_)
    {
      Container<IData>::operator=(std::move(other_));
    }

    return *this;
  }

  IAscanDataVector AscanData() const
  {
    IAscanDataVector ascanData;

    const auto items = TSuper::Select([&](const TItemPtr& item_) {
      if (auto ascanDataset = dynamic_cast<IAscanData*>(item_.get())) {
        return true;
      }
      else {
        return false;
      }
    });

    for (const auto item : items) {
      ascanData.push_back(dynamic_cast<const IAscanData*>(item));
    }

    return ascanData;
  }

  const IAscanData* ConfigAscanData(size_t configId_) const
  {
    return dynamic_cast<IAscanData*>(TSuper::Find([&configId_](const TItemPtr& item_)
      { return dynamic_cast<IAscanData*>(item_.get())->Source()->Id() == configId_; }));
  }

  //TIDatasets CScans(const std::wstring& configName_) const
  //{
  //  return TSuper::Select([&configName_](const TItemPtr& item_) {
  //    if (auto cscanDataset = dynamic_cast<ICscanDataset*>(item_.get())) {
  //      return cscanDataset->Configuration() == configName_;
  //    }
  //    else {
  //      return false;
  //    }
  //    });
  //}
};

using TDataMap = std::map<const std::filesystem::path, Datasets>;