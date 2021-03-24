#pragma once
#include <map>
#include <filesystem>
#include "Container.hpp"
#include "IReadOnlyData.h"
#include "IAscanDataVector.hpp"


class ReadOnlyDatasets : public ONDTLib::Container<IReadOnlyData>
{
  using TSuper = ONDTLib::Container<IReadOnlyData>;
public:
  ReadOnlyDatasets(const ReadOnlyDatasets&) = delete;
  ReadOnlyDatasets& operator=(const ReadOnlyDatasets&) = delete;
  ReadOnlyDatasets() = default;
  virtual ~ReadOnlyDatasets() {}

  ReadOnlyDatasets(ReadOnlyDatasets&& other_) noexcept
    : Container<IReadOnlyData>(std::move(other_))
  {
  }

  ReadOnlyDatasets& operator=(ReadOnlyDatasets&& other_) noexcept
  {
    if (this != &other_)
    {
      Container<IReadOnlyData>::operator=(std::move(other_));
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

  const IAscanData* AscanData(size_t configId_) const
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

using TDataMap = std::map<const std::filesystem::path, ReadOnlyDatasets>;