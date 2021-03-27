#pragma once
#include <map>
#include <string>
#include <filesystem>
#include "Data.h"
#include "Container.hpp"
#include "Ascans/AscanData.hpp"
#include "Cscans/CscanData.hpp"


class DataContainer : public ONDTLib::Container<Data>
{
  using TSuper = ONDTLib::Container<Data>;

public:
  DataContainer() = default;
  DataContainer(const std::string& fileVersion_)
    : m_fileVersion(fileVersion_)
  {
  }

  virtual ~DataContainer() = default;

  DataContainer(const DataContainer&) = delete;
  DataContainer& operator=(const DataContainer&) = delete;

  DataContainer(DataContainer&& other_) noexcept
    : Container<Data>(std::move(other_))
    , m_fileVersion(other_.m_fileVersion)
  {
  }

  DataContainer& operator=(DataContainer&& other_) noexcept
  {
    if (this != &other_)
    {
      Container<Data>::operator=(std::move(other_));
      m_fileVersion = other_.m_fileVersion;
    }

    return *this;
  }

  const std::string& Version() const {
    return m_fileVersion;
  }

  const AscanData* Ascan(size_t configId_) const
  {
    return dynamic_cast<const AscanData*>(TSuper::Find([&configId_](const TItemPtr& item_) {
      return item_->Source().ConfigId() == configId_ &&
        dynamic_cast<const AscanData*>(item_.get());
      }));
  }

  const CscanData* Cscan(size_t configId_) const
  {
    return dynamic_cast<const CscanData*>(TSuper::Find([&configId_](const TItemPtr& item_) {
      return item_->Source().ConfigId() == configId_ &&
        dynamic_cast<const CscanData*>(item_.get());
      }));
  }

private:
  std::string m_fileVersion;
};

using TDataContainerMap = std::map<const std::filesystem::path, DataContainer>;
