#pragma once
#include <string>
#include "Container.hpp"
#include "IReadOnlyData.h"
#include "IAscanData.h"


using TReadOnlyDatasets = std::vector<IReadOnlyData*>;
using TAscanDatasets = std::vector<const IAscanData*>;


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

  TAscanDatasets Ascans() const
  {
    TAscanDatasets datasets;

    const auto items = TSuper::Select([&](const TItemPtr& item_) {
      if (auto ascanDataset = dynamic_cast<IAscanData*>(item_.get())) {
        return true;
      }
      else {
        return false;
      }
    });

    for (const auto item : items) {
      datasets.push_back(dynamic_cast<const IAscanData*>(item));
    }

    return datasets;
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
