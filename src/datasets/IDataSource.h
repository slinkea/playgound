#pragma once
#include <vector>


class IDataset;
class IConfigurationSource;

class IDataSource
{
public:
  virtual ~IDataSource() = default;

  virtual const IConfigurationSource* Configuration() const = 0;
  virtual const std::vector<IDataset*> Datasets() const = 0;

};
