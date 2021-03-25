#pragma once
#include "AscanDataset.hpp"
#include "IAscanBeamDataset.h"


class AscanBeamDataset : public AscanDataset, public IAscanBeamDataset
{
public:
  AscanBeamDataset(hid_t Id_, const std::string& location_, size_t beamIdx_)
    : AscanDataset(Id_, location_)
    , m_beamIdx(beamIdx_)
  {
  }

  AscanBeamDataset() = delete;
  AscanBeamDataset(const AscanBeamDataset&) = delete;
  AscanBeamDataset& operator=(const AscanBeamDataset&) = delete;
  virtual ~AscanBeamDataset() = default;

  const std::string& Location() const {
    return AscanDataset::Location();
  };

  size_t BeamIndex() const override {
    return m_beamIdx;
  };

  const AscanAttributes& Attributes() const override {
    return AscanDataset::Attributes();
  }

  const DataDimensions& Dimensions() const {
    return AscanDataset::Dimensions();
  }

  const DatasetProperties& Properties() const override {
    return AscanDataset::Properties();
  }

  bool IsStatus() const override {
    return false;
  }

  bool IsData() const override {
    return true;
  }

  void SelectSingle(size_t x_, size_t y_) const override {
    return AscanDataset::SelectSingle(x_, y_);
  }

  void Read(void* dataOut_) const override {
    return AscanDataset::Read(dataOut_);
  }

protected:
  const size_t m_beamIdx{};  
};
