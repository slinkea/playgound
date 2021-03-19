#pragma once
#include <string>
#include "Container.hpp"
#include "IAscanDataSet.h"
#include "IAscanBeamDataSet.h"


using TIDataSets = std::vector<IDataSet*>;

class DataSets : public ONDTLib::Container<IDataSet>
{
  using TSuper = ONDTLib::Container<IDataSet>;
public:
  DataSets(const DataSets&) = delete;
  DataSets& operator=(const DataSets&) = delete;
  DataSets() = default;
  virtual ~DataSets() {}

  size_t Count(const std::string& name_) const
  {
    return TSuper::Count([&name_](const TItemPtr& item_)
      {
        if (auto ascanDataSet = dynamic_cast<IAscanDataSet*>(item_.get()))
          return ascanDataSet->Name() == name_;
        else if (auto ascanBeamDataSet = dynamic_cast<IAscanBeamDataSet*>(item_.get()))
          return ascanBeamDataSet->Name() == name_;

        return false;
      });
  }

  TIDataSets CScans(const std::string& name_) const
  {
    return TSuper::Select([&name_](const TItemPtr& item_)
    {
      if (auto cscanDataSet = dynamic_cast<CscanDataSet*>(item_.get()))
        return cscanDataSet->Name() == name_;
      else if (auto cscanBeamDataSet = dynamic_cast<CscanBeamDataSet*>(item_.get()))
        return cscanBeamDataSet->Name() == name_;

      return false;
    });
  }
};
