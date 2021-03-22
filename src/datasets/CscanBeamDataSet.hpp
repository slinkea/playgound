#pragma once
#include "CscanDataset.hpp"
#include "ICscanBeamDataset.h"


class CscanBeamDataset : public CscanDataset, public ICscanBeamDataset
{
public:
  CscanBeamDataset(hid_t id_, const std::string& location_, size_t beamIdx_, const std::wstring& sourceName_)
    : CscanDataset(id_, location_, sourceName_)
    , m_beamIdx(beamIdx_)
  {
  }

  CscanBeamDataset() = delete;
  CscanBeamDataset(const CscanBeamDataset&) = delete;
  CscanBeamDataset& operator=(const CscanBeamDataset&) = delete;
  virtual ~CscanBeamDataset() = default;

  const std::string& Location() const override {
    return CscanDataset::Location();
  };

  size_t BeamIndex() const override {
    return m_beamIdx;
  };

  const CscanAttributes& Attributes() const override {
    return CscanDataset::Attributes();
  }

  const CscanDataspace& Dataspace() const override {
    return CscanDataset::Dataspace();
  };

  void Read(const void* dataOut_) const override
  {
  }

protected:
  size_t m_beamIdx{};
};
