#pragma once
#include <string>
#include "Container.hpp"
#include "IReadOnlyData.h"


using TReadOnlyDatasets = std::vector<IReadOnlyData*>;

class ReadOnlyDatasets : public ONDTLib::Container<IReadOnlyData>
{
  using TSuper = ONDTLib::Container<IReadOnlyData>;
public:
  ReadOnlyDatasets(const ReadOnlyDatasets&) = delete;
  ReadOnlyDatasets& operator=(const ReadOnlyDatasets&) = delete;
  ReadOnlyDatasets() = default;
  virtual ~ReadOnlyDatasets() {}

  //TIDatasets AScans(const std::wstring& configName_) const
  //{
  //  return TSuper::Select([&configName_](const TItemPtr& item_) {
  //    if (auto ascanDataset = dynamic_cast<IAscanDataset*>(item_.get())) {
  //      return ascanDataset->Location() == configName_;
  //    }
  //    else {
  //      return false;
  //    }
  //  });
  //}

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
