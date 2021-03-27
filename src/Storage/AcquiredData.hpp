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

class AcquiredData
{
public:
  AcquiredData() = default;
  ~AcquiredData() = default;

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
      AscanDataSource ascanSource(filePath_,  configId.first, configId.second); //[TODO[EAB] Utiliser un id provenant de la config.]
      TAscanDataPtr ascanData = std::make_unique<AscanData>(ascanSource);
      FetchAscanData(configGroupId, ascanData);
      dataContainer.Add(std::move(ascanData));

      //CscanDataSource cscanSource(filePath_, configId.first, configId.second); //[TODO[EAB] Utiliser les ids provenant de la config.]
      //TCscanDataPtr cscanData = std::make_unique<CscanData>(cscanSource);
      //FetchCscanData(fileId, name, cscanData);
      //dataContainer.Add(std::move(cscanData));

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

  void FetchCscanData(hid_t fileId_, const std::string& configName_, TCscanDataPtr& cscanData_) const
  {
    std::stringstream configLocation;
    configLocation << GROUP_DATA << "/" << configName_ << "/";

    std::stringstream cscanLocation;
    cscanLocation << configLocation.str() << CSCAN_GROUP << "/";

    herr_t status = H5Gget_objinfo(fileId_, cscanLocation.str().c_str(), 0, nullptr);
    if (status == 0)
    {
      hid_t cscanGroupId = H5Gopen(fileId_, cscanLocation.str().c_str(), H5P_DEFAULT);
      if (cscanGroupId >= 0)
      {
        hsize_t gateQty;
        if (H5Gget_num_objs(cscanGroupId, &gateQty) >= 0)
        {
          char dsetName[MAX_NAME_LENGTH];
          for (size_t gateIdx(0); gateIdx < gateQty; gateIdx++)
          {
            ssize_t nameLength = H5Gget_objname_by_idx(cscanGroupId, gateIdx, dsetName, MAX_NAME_LENGTH);
            if (nameLength > 0)
            {
              std::string gateName(dsetName);
              TGateIdentifiers gateIds = { {gateIdx, std::wstring(gateName.begin(), gateName.end())} };

              std::stringstream dsetLocation;
              dsetLocation << cscanLocation.str() << gateName;
              hid_t dsetId = H5Dopen(fileId_, dsetLocation.str().c_str(), H5P_DEFAULT);
              cscanData_->Datasets().Add(std::make_unique<CscanDataset>(dsetId, dsetLocation.str(), gateIds));
            }
          }
        }

        H5Gclose(cscanGroupId);
      }
    }
    else
    {
      herr_t status = H5Gget_objinfo(fileId_, configLocation.str().c_str(), 0, nullptr);
      if (status == 0)
      {
        hid_t configGroupId = H5Gopen(fileId_, configLocation.str().c_str(), H5P_DEFAULT);
        if (configGroupId >= 0)
        {
          hsize_t beamQty{};
          if (H5Gget_num_objs(configGroupId, &beamQty) >= 0)
          {
            for (size_t beamIdx{}; beamIdx < beamQty; beamIdx++)
            {
              std::stringstream cscanLocation;
              std::string beamStr(BEAM_PREFIX + std::string(" ") + std::to_string(beamIdx + 1));
              cscanLocation << configLocation.str() << beamStr << "/" << CSCAN_GROUP << "/";

              //

              hid_t cscanId = H5Gopen(fileId_, cscanLocation.str().c_str(), H5P_DEFAULT);
              if (cscanId >= 0)
              {
                hsize_t gateQty;
                if (H5Gget_num_objs(cscanId, &gateQty) >= 0)
                {
                  char dsetName[MAX_NAME_LENGTH];
                  for (size_t gateIdx(0); gateIdx < gateQty; gateIdx++)
                  {
                    ssize_t nameLength = H5Gget_objname_by_idx(cscanId, gateIdx, dsetName, MAX_NAME_LENGTH);
                    if (nameLength > 0)
                    {
                      std::string gateName(dsetName);
                      TGateIdentifiers gateIds = { {gateIdx, std::wstring(gateName.begin(), gateName.end())} };

                      std::stringstream dsetLocation;
                      dsetLocation << cscanLocation.str() << gateName;
                      hid_t dsetId = H5Dopen(fileId_, dsetLocation.str().c_str(), H5P_DEFAULT);
                      cscanData_->Datasets().Add(std::make_unique<CscanBeamDataset>(dsetId, dsetLocation.str(), beamIdx, gateIds));
                    }
                  }
                }

                H5Gclose(cscanId);
              }
            }
          }

          H5Gclose(configGroupId);
        }
      }
    }    
  }

  TDataContainerMap m_dataContainers;
  using TFileMap = std::map<const fs::path, const hid_t>;
  TFileMap m_fileMap;
};
