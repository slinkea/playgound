#pragma once
#include <map>
#include <filesystem>
#include "ascans/IAscanDataVector.hpp"
#include "Container.hpp"
#include "IData.h"


class DataContainer : public ONDTLib::Container<IData>
{
  using TSuper = ONDTLib::Container<IData>;

public:
  DataContainer(const DataContainer&) = delete;
  DataContainer& operator=(const DataContainer&) = delete;
  DataContainer() = default;
  virtual ~DataContainer() {}

  DataContainer(DataContainer&& other_) noexcept
    : Container<IData>(std::move(other_))
  {
  }

  DataContainer& operator=(DataContainer&& other_) noexcept
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

  const IAscanData* ConfigData(size_t configId_) const
  {
    return dynamic_cast<IAscanData*>(TSuper::Find([&configId_](const TItemPtr& item_)
      { return dynamic_cast<IAscanData*>(item_.get())->Source()->Id() == configId_; }));
  }
};

using TDataMap = std::map<const std::filesystem::path, DataContainer>;