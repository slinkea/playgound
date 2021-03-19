#pragma once
#include "AscanDataSet.hpp"
#include "IAscanDataSet.h"


class AscanMergedBeamDataSet : public AscanDataSet, public IAscanDataSet
{
public:
  AscanMergedBeamDataSet(hid_t id_, const std::wstring& configName_)
  : AscanDataSet(id_, configName_)
  {
  }

  AscanMergedBeamDataSet() = delete;
  AscanMergedBeamDataSet(const AscanMergedBeamDataSet&) = delete;
  AscanMergedBeamDataSet& operator=(const AscanMergedBeamDataSet&) = delete;

  virtual ~AscanMergedBeamDataSet() = default;

  const std::wstring& ConfigName() const override {
    return DataSet::ConfigName();
  };

  const AscanAttributes& Attributes() const override {
    return AscanDataSet::Attributes();
  }

  const AscanDataSpace& DataSpace() const override {
    return AscanDataSet::DataSpace();
  };

  void Read(const void* dataOut_) const override
  {
  }

protected:
  void Fetch()
  {

  }
};
