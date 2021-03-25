#pragma once
#include <map>
#include <string>
#include <filesystem>
#include "IData2.h"
#include "Container.hpp"
#include "DatasetContainer2.hpp"


class DataContainer2 : public ONDTLib::Container<IData2>
{
  using TSuper = ONDTLib::Container<IData2>;

public:
  DataContainer2(const std::string& fileVersion_)
    : m_fileVersion(fileVersion_)
  {
  }

  virtual ~DataContainer2() = default;

  DataContainer2(const DataContainer2&) = delete;
  DataContainer2& operator=(const DataContainer2&) = delete;

  DataContainer2(DataContainer2&& other_) noexcept
    : Container<IData2>(std::move(other_))
    , m_fileVersion(other_.m_fileVersion)
  {
  }

  DataContainer2& operator=(DataContainer2&& other_) noexcept
  {
    if (this != &other_)
    {
      Container<IData2>::operator=(std::move(other_));
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

using TDataContainerMap = std::map<const std::filesystem::path, DataContainer2>;
