#pragma once
#include <map>
#include <string>
#include <filesystem>
#include "ascans/IAscanDataVector.hpp"
#include "Container.hpp"
#include "IData.h"


class DataContainer : public ONDTLib::Container<IData>
{
  using TSuper = ONDTLib::Container<IData>;

public:
  DataContainer(const std::string& fileVersion_)
    : m_fileVersion(fileVersion_)
  {
  }

  DataContainer(const DataContainer&) = delete;
  DataContainer& operator=(const DataContainer&) = delete;
  virtual ~DataContainer() {}

  DataContainer(DataContainer&& other_) noexcept
    : Container<IData>(std::move(other_))
    , m_fileVersion(other_.m_fileVersion)
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

  const std::string& Version() const {
    return m_fileVersion;
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

private:
  const std::string m_fileVersion;
};

using TDataMap = std::map<const std::filesystem::path, DataContainer>;