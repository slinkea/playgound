#pragma once
#include <string>
#include "Container.hpp"
#include "IAscanDataset.h"
#include "ICscanDataset.h"


using TIDatasets = std::vector<IDataset*>;

class Datasets : public ONDTLib::Container<IDataset>
{
  using TSuper = ONDTLib::Container<IDataset>;
public:
  Datasets(const Datasets&) = delete;
  Datasets& operator=(const Datasets&) = delete;
  Datasets() = default;
  virtual ~Datasets() {}

  TIDatasets AScans(const std::wstring& configName_) const
  {
    return TSuper::Select([&configName_](const TItemPtr& item_) {
      if (auto ascanDataset = dynamic_cast<IAscanDataset*>(item_.get())) {
        return ascanDataset->Configuration() == configName_;
      }
      else {
        return false;
      }
    });
  }

  TIDatasets CScans(const std::wstring& configName_) const
  {
    return TSuper::Select([&configName_](const TItemPtr& item_) {
      if (auto cscanDataset = dynamic_cast<ICscanDataset*>(item_.get())) {
        return cscanDataset->Configuration() == configName_;
      }
      else {
        return false;
      }
      });
  }
};
