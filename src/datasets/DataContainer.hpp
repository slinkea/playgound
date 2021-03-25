#pragma once
#include <map>
#include <string>
#include <filesystem>
#include "IData.h"
#include "Container.hpp"
#include "DatasetContainer.hpp"


class DataContainer : public ONDTLib::Container<IData>
{
  using TSuper = ONDTLib::Container<IData>;

public:
  DataContainer(const std::string& fileVersion_)
    : m_fileVersion(fileVersion_)
  {
  }

  virtual ~DataContainer() = default;

  DataContainer(const DataContainer&) = delete;
  DataContainer& operator=(const DataContainer&) = delete;

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
      m_fileVersion = other_.m_fileVersion;
    }

    return *this;
  }

  const std::string& Version() const {
    return m_fileVersion;
  }

private:
  std::string m_fileVersion;
};

using TDataContainerMap = std::map<const std::filesystem::path, DataContainer>;
