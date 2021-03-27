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
constexpr char FILE_VERSION_1_2_0[] = "1.2.0";

class NdtData
{
public:
  NdtData() = default;
  ~NdtData() = default;

  void Open(const fs::path& filePath_)
  {
    H5_RESULT_CHECK(H5Eset_auto(H5E_DEFAULT, NULL, NULL));
    hid_t fileId = H5Fopen(filePath_.string().c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    m_fileMap.emplace(filePath_, fileId);

    const auto fileVersion = H5Utils::GetFileVersion(fileId);
    DataContainer container(fileVersion);

    const auto configIds = H5Utils::GetConfigurations(fileId);

    if (VersionUtils::IsEqualOrLess(fileVersion, FILE_VERSION_1_2_0)) {
      FetchDataVersion120(filePath_, configIds, std::move(container));
    }
  }

  void Close(const fs::path& filePath_)
  {
    m_dataContainers.erase(filePath_);
    H5_RESULT_CHECK(H5Fclose(m_fileMap[filePath_]));
  }

  const DataContainer& GetDataContainer(const fs::path& filePath_) const {
    return m_dataContainers.at(filePath_);
  }

  DataContainer& GetDataContainer(const fs::path& filePath_) {
    return m_dataContainers.at(filePath_);
  }

private:
  void FetchDataVersion120(const fs::path& filePath_, const TConfigMap& configIds, DataContainer&& dataContainer)
  {
    for (const auto& configId : configIds)
    {
      std::wstringstream location;
      location << GROUP_DATA << "/" << configId.second << "/";

#pragma warning( push )
#pragma warning( disable: 4996 )
      std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
      std::string dataLoc = conv.to_bytes(location.str());
#pragma warning( pop )
      
      hid_t configGroupId = H5Gopen(m_fileMap[filePath_], dataLoc.c_str(), H5P_DEFAULT);
      AscanDataSource ascanSource(filePath_,  configId.first, configId.second);
      TAscanDataPtr ascanData = std::make_unique<AscanData>(ascanSource);
      FetchAscanData(configGroupId, ascanData);
      dataContainer.Add(std::move(ascanData));

      CscanDataSource cscanSource(filePath_, configId.first, configId.second);
      TCscanDataPtr cscanData = std::make_unique<CscanData>(cscanSource);
      FetchCscanData(configGroupId, cscanData);
      dataContainer.Add(std::move(cscanData));

      H5Gclose(configGroupId);
    }

    m_dataContainers.emplace(std::make_pair(filePath_, std::move(dataContainer)));
  }

  void FetchAscanData(hid_t configGroupId_, TAscanDataPtr& ascanData_) const
  {
    herr_t status = H5Gget_objinfo(configGroupId_, ASCAN_DATASET, 0, nullptr);
    if (status == 0) {
      ascanData_->Datasets().Add(std::make_unique<AscanDataset>(configGroupId_));
    }
    else {
      hsize_t beamQty{};
      if (H5Gget_num_objs(configGroupId_, &beamQty) >= 0) {
        for (size_t beamIdx(1); beamIdx <= beamQty; beamIdx++)
        {
          std::string beamStr(BEAM_PREFIX + std::string(" ") + std::to_string(beamIdx));
          hid_t beamGroupId = H5Gopen(configGroupId_, beamStr.c_str(), H5P_DEFAULT);
          ascanData_->Datasets().Add(std::make_unique<AscanBeamDataset>(beamGroupId, beamIdx));
          H5Gclose(beamGroupId);
        }
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

  void FetchCscanData(hid_t configGroupId_, TCscanDataPtr& cscanData_) const
  {
    herr_t status = H5Gget_objinfo(configGroupId_, CSCAN_GROUP, 0, nullptr);
    if (status == 0) {
      hid_t cscanGroupId = H5Gopen(configGroupId_, CSCAN_GROUP, H5P_DEFAULT);
      FetchCscanDatasets(cscanGroupId, cscanData_);
      H5Gclose(cscanGroupId);
    }
    else {
      hsize_t beamQty{};
      if (H5Gget_num_objs(configGroupId_, &beamQty) >= 0) {
        for (size_t beamIdx(1); beamIdx <= beamQty; beamIdx++)
        {
          std::string beamStr(BEAM_PREFIX + std::string(" ") + std::to_string(beamIdx));
          hid_t beamGroupId = H5Gopen(configGroupId_, beamStr.c_str(), H5P_DEFAULT);
          hid_t cscanGroupId = H5Gopen(beamGroupId, CSCAN_GROUP, H5P_DEFAULT);
          FetchCscanDatasets(cscanGroupId, cscanData_, beamIdx);
          H5Gclose(cscanGroupId);
          H5Gclose(beamGroupId);
        }
      }
    }
  }

  TDataContainerMap m_dataContainers;
  std::map<fs::path, hid_t> m_fileMap;
};
