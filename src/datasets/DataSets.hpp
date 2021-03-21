#pragma once
#include <string>
#include "Container.hpp"
#include "IAscanDataSet.h"
#include "ICscanDataSet.h"


using TIDataSets = std::vector<IDataSet*>;

class DataSets : public ONDTLib::Container<IDataSet>
{
  using TSuper = ONDTLib::Container<IDataSet>;
public:
  DataSets(const DataSets&) = delete;
  DataSets& operator=(const DataSets&) = delete;
  DataSets() = default;
  virtual ~DataSets() {}

  TIDataSets AScans(const std::wstring& configName_) const
  {
    return TSuper::Select([&configName_](const TItemPtr& item_) {
      if (auto ascanDataSet = dynamic_cast<IAscanDataSet*>(item_.get())) {
        return ascanDataSet->ConfigName() == configName_;
      }
      else {
        return false;
      }
    });
  }

  TIDataSets CScans(const std::wstring& configName_) const
  {
    return TSuper::Select([&configName_](const TItemPtr& item_) {
      if (auto cscanDataSet = dynamic_cast<ICscanDataSet*>(item_.get())) {
        return cscanDataSet->ConfigName() == configName_;
      }
      else {
        return false;
      }
      });
  }
};
