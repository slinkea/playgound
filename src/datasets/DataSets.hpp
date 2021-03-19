#pragma once
#include "Container.hpp"
#include "IAscanDataSet.h"
#include "IAscanBeamDataSet.h"


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
};
