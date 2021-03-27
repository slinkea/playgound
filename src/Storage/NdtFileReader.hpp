#pragma once
#include <map>
#include <regex>
#include <vector>
#include <string>
#include <sstream>
#include <codecvt>
#include <filesystem>
#include <hdf5/hdf5.h>

#include "H5Utils.hpp"
#include "VersionUtils.hpp"
#include "DataContainer.hpp"
#include "Ascans/AscanData.hpp"
#include "Cscans/CscanData.hpp"


namespace fs = std::filesystem;

// Supported version
constexpr char FILE_VERSION_1_2_0[] = "1.2.0";

class NdtFileReader
{
public:
  NdtFileReader() = default;

  NdtFileReader(const fs::path& filePath_) {
    Open(filePath_);
  }

  virtual ~NdtFileReader() {
    Close();
  }

  void Open(const fs::path& filePath_)
  {
    H5_RESULT_CHECK(H5Eset_auto(H5E_DEFAULT, NULL, NULL));
    m_fileId = H5Fopen(filePath_.string().c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);

    const auto fileVersion = H5Utils::GetFileVersion(m_fileId);
    m_dataContainer = DataContainer(fileVersion);

    const auto configIdsMap = H5Utils::GetConfigurations(m_fileId);

    if (VersionUtils::IsEqualOrLess(fileVersion, FILE_VERSION_1_2_0)) {
      FetchDataVersion120(filePath_, configIdsMap);
    }
  }

  void Close()
  {
    if (m_fileId > 0)
    {
      H5_RESULT_CHECK(H5Fclose(m_fileId));
      m_fileId = 0;
    }
  }

  bool IsOpen() const {
    return m_fileId != 0;
  }

  const DataContainer& Data() const {
    return m_dataContainer;
  }

private:
  void FetchDataVersion120(const fs::path& filePath_, const TConfigIdsMap& configIdsMap_)
  {
    for (const auto& configIds : configIdsMap_)
    {
      std::wstringstream location;
      location << GROUP_DATA << "/" << configIds.second << "/";

#pragma warning( push )
#pragma warning( disable: 4996 )
      std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
      std::string dataLoc = conv.to_bytes(location.str());
#pragma warning( pop )
      
      hid_t configGroupId = H5Gopen(m_fileId, dataLoc.c_str(), H5P_DEFAULT);
      FetchAscanData(filePath_, configGroupId, configIds);
      FetchCscanData(filePath_, configGroupId, configIds);
      H5Gclose(configGroupId);
    }
  }

  void FetchAscanData(const fs::path& filePath_, hid_t configGroupId_, const TConfigIdPair& configIds_)
  {
    herr_t status = H5Gget_objinfo(configGroupId_, ASCAN_DATASET, 0, nullptr);
    if (status == 0) 
    {
      AscanDataSource ascanSource(filePath_, configIds_, true);
      TAscanDataPtr ascanData = std::make_unique<AscanData>(ascanSource);
      ascanData->Datasets().Add(std::make_unique<AscanDataset>(configGroupId_));
      m_dataContainer.Add(std::move(ascanData));
    }
    else {
      hsize_t beamQty{};
      if (H5Gget_num_objs(configGroupId_, &beamQty) >= 0) 
      {
        AscanDataSource ascanSource(filePath_, configIds_, false);
        TAscanDataPtr ascanData = std::make_unique<AscanData>(ascanSource);

        for (size_t beamIdx(1); beamIdx <= beamQty; beamIdx++)
        {
          std::string beamStr(BEAM_PREFIX + std::string(" ") + std::to_string(beamIdx));
          hid_t beamGroupId = H5Gopen(configGroupId_, beamStr.c_str(), H5P_DEFAULT);
          ascanData->Datasets().Add(std::make_unique<AscanBeamDataset>(beamGroupId, beamIdx));
          H5Gclose(beamGroupId);
        }

        m_dataContainer.Add(std::move(ascanData));
      }
    }
  }

  void FetchCscanDatasets(hid_t cscanGroupId_, TCscanDataPtr& cscanData_, size_t beamIdx_ = 0) const
  {
    hsize_t gateQty;
    char dsetName[MAX_NAME_LENGTH];

    if (H5Gget_num_objs(cscanGroupId_, &gateQty) >= 0) {
      for (size_t gateIdx{}; gateIdx < gateQty; gateIdx++)
      {
        ssize_t nameLength = H5Gget_objname_by_idx(cscanGroupId_, gateIdx, dsetName, MAX_NAME_LENGTH);
        if (nameLength > 0)
        {
          std::string gateName(dsetName);
          TGateIdentifiers gateIds = { {gateIdx, std::wstring(gateName.begin(), gateName.end())} };
          hid_t dsetId = H5Dopen(cscanGroupId_, dsetName, H5P_DEFAULT);

          if (beamIdx_ == 0) {
            cscanData_->Datasets().Add(std::make_unique<CscanDataset>(dsetId, gateIds));
          }
          else {
            cscanData_->Datasets().Add(std::make_unique<CscanBeamDataset>(dsetId, gateIds, beamIdx_));
          }
        }
      }
    }
  }

  void FetchCscanData(const fs::path& filePath_, hid_t configGroupId_, const TConfigIdPair& configIds_)
  {
    herr_t status = H5Gget_objinfo(configGroupId_, CSCAN_GROUP, 0, nullptr);
    if (status == 0) 
    {
      CscanDataSource cscanSource(filePath_, configIds_, true);
      TCscanDataPtr cscanData = std::make_unique<CscanData>(cscanSource);

      hid_t cscanGroupId = H5Gopen(configGroupId_, CSCAN_GROUP, H5P_DEFAULT);
      FetchCscanDatasets(cscanGroupId, cscanData);
      H5Gclose(cscanGroupId);

      m_dataContainer.Add(std::move(cscanData));
    }
    else 
    {
      hsize_t beamQty{};
      if (H5Gget_num_objs(configGroupId_, &beamQty) >= 0)
      {
        CscanDataSource cscanSource(filePath_, configIds_, false);
        TCscanDataPtr cscanData = std::make_unique<CscanData>(cscanSource);

        for (size_t beamIdx(1); beamIdx <= beamQty; beamIdx++)
        {
          std::string beamStr(BEAM_PREFIX + std::string(" ") + std::to_string(beamIdx));
          hid_t beamGroupId = H5Gopen(configGroupId_, beamStr.c_str(), H5P_DEFAULT);
          hid_t cscanGroupId = H5Gopen(beamGroupId, CSCAN_GROUP, H5P_DEFAULT);
          FetchCscanDatasets(cscanGroupId, cscanData, beamIdx);
          H5Gclose(cscanGroupId);
          H5Gclose(beamGroupId);
        }

        m_dataContainer.Add(std::move(cscanData));
      }
    }
  }

  hid_t m_fileId{};
  DataContainer m_dataContainer;
};
