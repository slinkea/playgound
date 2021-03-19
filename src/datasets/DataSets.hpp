#pragma once
#include <string>
#include "Container.hpp"
#include "IAscanDataSet.h"
#include "IAscanBeamDataSet.h"
#include "ICscanDataSet.h"
#include "ICscanBeamDataSet.h"


using TIDataSets = std::vector<IDataSet*>;

class DataSets : public ONDTLib::Container<IDataSet>
{
  using TSuper = ONDTLib::Container<IDataSet>;
public:
  DataSets(const DataSets&) = delete;
  DataSets& operator=(const DataSets&) = delete;
  DataSets() = default;
  virtual ~DataSets() {}

  size_t Count(const std::wstring& configName_) const
  {
    return TSuper::Count([&configName_](const TItemPtr& item_)
      {
        if (auto ascanDataSet = dynamic_cast<IAscanDataSet*>(item_.get()))
          return ascanDataSet->ConfigName() == configName_;
        else if (auto ascanBeamDataSet = dynamic_cast<IAscanBeamDataSet*>(item_.get()))
          return ascanBeamDataSet->ConfigName() == configName_;

        return false;
      });
  }

  TIDataSets CScans(const std::wstring& configName_) const
  {
    return TSuper::Select([&configName_](const TItemPtr& item_)
    {
      if (auto cscanDataSet = dynamic_cast<ICscanDataSet*>(item_.get()))
        return cscanDataSet->ConfigName() == configName_;
      else if (auto cscanBeamDataSet = dynamic_cast<ICscanBeamDataSet*>(item_.get()))
        return cscanBeamDataSet->ConfigName() == configName_;

      return false;
    });
  }
};
