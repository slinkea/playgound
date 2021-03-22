#pragma once
#include <vector>
#include "IDataSource.h"
#include "PhasedArraySource.hpp"
#include "Dataset.hpp"


class PhasedArraySource;

class AscanDataSource : public IDataSource
{
public:
  AscanDataSource(const PhasedArraySource* configSource_, const std::vector<IDataset*>& dataset_)
    : m_configSource(configSource_)
    , m_dataset(dataset_)
  {
  }

  AscanDataSource(const AscanDataSource&) = delete;
  AscanDataSource& operator=(const AscanDataSource&) = delete;

  virtual ~AscanDataSource() = default;

  const IConfigurationSource* Configuration() const override {
    return m_configSource;
  }

  const std::vector<IDataset*> Datasets() const override {
    return m_dataset;
  }

private:
  const std::vector<IDataset*> m_dataset;
  const PhasedArraySource* m_configSource;
};
