#pragma once
#include <map>
#include <regex>
#include <vector>
#include <string>
#include <sstream>
#include <filesystem>
#include <hdf5/hdf5.h>

#include "DataContainer.hpp"
#include "Ascans/AscanData.hpp"
#include "Cscans/CscanData.hpp"


namespace fs = std::filesystem;

constexpr char FLE_VERSION[] = "File Version";
constexpr char FILE_VERSION_1_2_0[] = "1.2.0";


struct VersionNumber
{
  VersionNumber(size_t major_, size_t minor_, size_t rev_)
    : Major(major_)
    , Minor(minor_)
    , Revision(rev_)
  {
  }

  bool IsEqualOrLower(size_t major_, size_t minor_, size_t rev_) const
  {
    if (Major == 1)
    {
      if (major_ <= Major)
      {
        if (minor_ <= Minor)
        {

        }
      }
    }
    
    return false;
  }

  size_t Major{};
  size_t Minor{};
  size_t Revision{};
};

class AcquiredData
{
public:
  AcquiredData() = default;
  ~AcquiredData() = default;

  void Open(const fs::path& filePath_)
  {
    H5_RESULT_CHECK(H5Eset_auto(H5E_DEFAULT, NULL, NULL));
    hid_t fileId = H5Fopen(filePath_.string().c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    m_h5FileMap.emplace(filePath_, fileId);

    const auto fileVersion = GetFileVersion(filePath_);
    DataContainer container(fileVersion);

    if (IsEqualOrLess(fileVersion, FILE_VERSION_1_2_0)) {
      FetchDataVersion120(filePath_, std::move(container));
    }
  }

  void Close(const fs::path& filePath_)
  {
    m_datacontainers.erase(filePath_);
    H5_RESULT_CHECK(H5Fclose(m_h5FileMap[filePath_]));
  }

  static size_t Size(const fs::path& filePath_) 
  {
    hsize_t fileSize{};
    H5_RESULT_CHECK(H5Eset_auto(H5E_DEFAULT, NULL, NULL));
    hid_t fileId = H5Fopen(filePath_.string().c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    H5_RESULT_CHECK(H5Fget_filesize(fileId, &fileSize));
    H5_RESULT_CHECK(H5Fclose(fileId));

    return fileSize;
  }

  const DataContainer& GetDataContainer(const fs::path& filePath_) const {
    return m_datacontainers.at(filePath_);
  }

  DataContainer& GetDataContainer(const fs::path& filePath_) {
    return m_datacontainers.at(filePath_);
  }

private:
  void Parse(int result[3], const std::string& input_)
  {
    std::istringstream parser(input_);
    parser >> result[0];
    for (int idx(1); idx < 3; idx++)
    {
      parser.get(); //Skip period
      parser >> result[idx];
    }
  }

  bool IsEqualOrLess(const std::string& a_, const std::string& b_)
  {
    if (a_ == b_) {
      return true;
    }

    int parsedA[3], parsedB[3];
    Parse(parsedA, a_);
    Parse(parsedB, b_);
    return std::lexicographical_compare(parsedB, parsedB + 3, parsedA, parsedA + 3);
  }

  const std::string GetFileVersion(const fs::path& filePath_) const
  {
    char fileVersion[MAX_NAME_LENGTH]{};
    hid_t fileId = m_h5FileMap.at(filePath_);
    hid_t attrId = H5Aopen(fileId, FLE_VERSION, H5P_DEFAULT);
    hid_t attrType = H5Aget_type(attrId);
    H5_RESULT_CHECK(H5Aread(attrId, attrType, fileVersion));
    H5Aclose(attrId);

    return std::string(fileVersion);
  }

  void FetchDataVersion120(const fs::path& filePath_, DataContainer&& dataContainer)
  {
    hsize_t groupQty{};
    char groupName[MAX_NAME_LENGTH];
    hid_t dataGroupId = H5Gopen(m_h5FileMap[filePath_], GROUP_DATA, H5P_DEFAULT);
    if (H5Gget_num_objs(dataGroupId, &groupQty) >= 0)
    {
      for (hsize_t groupIdx{}; groupIdx < groupQty; groupIdx++)
      {
        ssize_t nameLength = H5Gget_objname_by_idx(dataGroupId, groupIdx, groupName, MAX_NAME_LENGTH);
        if (nameLength > 0) 
        {
          std::string name(groupName);
          std::wstring configName(name.begin(), name.end());
          hid_t fileId = m_h5FileMap.at(filePath_);

          AscanDataSource ascanSource(filePath_, groupIdx, configName); //[TODO[EAB] Utiliser un id provenant de la config.]
          TAscanDataPtr ascanData = std::make_unique<AscanData>(ascanSource);
          FetchAscanData(fileId, name, ascanData);
          dataContainer.Add(std::move(ascanData));

          CscanDataSource cscanSource(filePath_, groupIdx, configName); //[TODO[EAB] Utiliser les ids provenant de la config.]
          TCscanDataPtr cscanData = std::make_unique<CscanData>(cscanSource);
          FetchCscanData(fileId, name, cscanData);
          dataContainer.Add(std::move(cscanData));
        }
      }

      m_datacontainers.emplace(std::make_pair(filePath_, std::move(dataContainer)));
    }

    H5Gclose(dataGroupId);
  }

  void FetchAscanDatasets(hid_t fileId_, const std::string& location_, TAscanDataPtr& ascanData_) const
  {
    std::stringstream dataLocation;
    dataLocation << location_ << ASCAN_DATASET;
    hid_t dsetId = H5Dopen(fileId_, dataLocation.str().c_str(), H5P_DEFAULT);
    ascanData_->Datasets().Add(std::make_unique<AscanDataset>(dsetId, location_));

    std::stringstream statusLocation;
    statusLocation << location_ << ASCAN_STATUS_DATASET;
    dsetId = H5Dopen(fileId_, statusLocation.str().c_str(), H5P_DEFAULT);
    ascanData_->Datasets().Add(std::make_unique<AscanStatusDataset>(dsetId, location_));
  }

  void FetchAscanBeamDatasets(hid_t fileId_, const std::string& location_, hsize_t beamQty_, TAscanDataPtr& ascanData_) const
  {
    for (size_t beamIdx{}; beamIdx < beamQty_; beamIdx++)
    {
      std::stringstream beamLocation;
      std::string beamStr(BEAM_PREFIX + std::string(" ") + std::to_string(beamIdx + 1));
      beamLocation << location_ << beamStr << "/";

      herr_t status = H5Gget_objinfo(fileId_, beamLocation.str().c_str(), 0, nullptr);
      if (status == 0)
      {
        std::stringstream dataLocation;
        dataLocation << beamLocation.str() << ASCAN_DATASET;
        hid_t dsetId = H5Dopen(fileId_, dataLocation.str().c_str(), H5P_DEFAULT);
        ascanData_->Datasets().Add(std::make_unique<AscanBeamDataset>(dsetId, location_, beamIdx));

        std::stringstream statusLocation;
        statusLocation << beamLocation.str() << ASCAN_STATUS_DATASET;
        dsetId = H5Dopen(fileId_, statusLocation.str().c_str(), H5P_DEFAULT);
        ascanData_->Datasets().Add(std::make_unique<AscanStatusBeamDataset>(dsetId, location_, beamIdx));
      }
    }
  }

  void FetchAscanData(hid_t fileId_, const std::string& configName_, TAscanDataPtr& ascanData_) const
  {
    std::stringstream dataLocation;
    dataLocation << GROUP_DATA << "/" << configName_ << "/";

    hid_t configGroupId = H5Gopen(fileId_, dataLocation.str().c_str(), H5P_DEFAULT);
    if (configGroupId >= 0)
    {
      herr_t status = H5Gget_objinfo(configGroupId, ASCAN_DATASET, 0, nullptr);
      if (status == 0)
      {
        FetchAscanDatasets(fileId_, dataLocation.str(), ascanData_);
      }
      else
      {
        hsize_t beamQty{};
        if (H5Gget_num_objs(configGroupId, &beamQty) >= 0)
        {
          FetchAscanBeamDatasets(fileId_, dataLocation.str(), beamQty, ascanData_);
        }
      }

      H5Gclose(configGroupId);
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
              TGateIdentifiers gateIds;
              std::stringstream dsetLocation;
              dsetLocation << cscanLocation.str() << std::string(dsetName);
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
                      cscanData_->Datasets().Add(std::make_unique<CscanDataset>(dsetId, dsetLocation.str(), gateIds));
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

  TDataContainerMap m_datacontainers;

  using TH5FileMap = std::map<const fs::path, const hid_t>;
  TH5FileMap m_h5FileMap;
};
