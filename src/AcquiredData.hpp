#pragma once
#include <map>
#include <regex>
#include <vector>
#include <string>
#include <sstream>
#include <filesystem>
#include <hdf5/hdf5.h>
#include "datasets/Datasets.hpp"
#include "datasets/AscanBeamDataset.hpp"
#include "datasets/AscanStatusBeamDataset.hpp"
#include "datasets/AscanMergedBeamDataset.hpp"
#include "datasets/AscanStatusMergedBeamDataset.hpp"
#include "datasets/AscanData.hpp"
#include "datasets/IAscanDatasetVector.hpp"
#include "datasets/AscanDataSource.hpp"


namespace fs = std::filesystem;

constexpr char FLE_VERSION[] = "File Version";
constexpr char FILE_VERSION_1_2_0[] = "1.2.0";


#define H5_RESULT_CHECK( ret )                \
    if ( ret < 0 ) {                          \
    throw std::runtime_error("HDF5 Error.");  \
    }

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
    if (IsEqualOrLess(fileVersion, FILE_VERSION_1_2_0))
    {
      FetchData_120(filePath_);
    }
  }

  void Close(const fs::path& filePath_)
  {
    m_datasetsMap.erase(filePath_);
    H5_RESULT_CHECK(H5Fclose(m_h5FileMap[filePath_]));
  }

  const ReadOnlyDatasets& Datasets(const fs::path& filePath_) const {
    return m_datasetsMap.at(filePath_);
  }

  ReadOnlyDatasets& Datasets(const fs::path& filePath_) {
    return m_datasetsMap.at(filePath_);
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

  void FetchData_120(const fs::path& filePath_)
  {
    hsize_t groupQty{};
    char name[MAX_NAME_LENGTH];
    hid_t dataGroupId = H5Gopen(m_h5FileMap[filePath_], GROUP_DATA, H5P_DEFAULT);
    if (H5Gget_num_objs(dataGroupId, &groupQty) >= 0)
    {
      ReadOnlyDatasets datasets;
      for (hsize_t groupIdx{}; groupIdx < groupQty; groupIdx++)
      {
        ssize_t nameLength = H5Gget_objname_by_idx(dataGroupId, groupIdx, name, MAX_NAME_LENGTH);
        if (nameLength > 0) {
          GetAscanData(filePath_, groupIdx, std::string(name), datasets);  //[TODO[EAB] Utiliser un id provenant de la config.]
        }
      }

      m_datasetsMap.emplace(std::make_pair(filePath_, std::move(datasets)));
    }

    H5Gclose(dataGroupId);
  }

  const IAscanDatasetVector GetAscanMergedBeamDatasets(hid_t fileId_, const std::string& location_) const
  {
    IAscanDatasetVector datasets;

    std::stringstream dataLocation;
    dataLocation << location_ << ASCAN_DATASET;
    hid_t dsetId = H5Dopen(fileId_, dataLocation.str().c_str(), H5P_DEFAULT);
    datasets.push_back(std::make_shared<AscanMergedBeamDataset>(dsetId, dataLocation.str()));

    std::stringstream statusLocation;
    statusLocation << location_ << ASCAN_STATUS_DATASET;
    dsetId = H5Dopen(fileId_, statusLocation.str().c_str(), H5P_DEFAULT);
    datasets.push_back(std::make_shared<AscanStatusMergedBeamDataset>(dsetId, dataLocation.str()));

    return datasets;
  }

  const IAscanDatasetVector GetAscanBeamDatasets(hid_t fileId_, const std::string& location_, hsize_t beamQty_) const
  {
    IAscanDatasetVector datasets;

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
        datasets.push_back(std::make_shared<AscanBeamDataset>(dsetId, dataLocation.str(), beamIdx));

        std::stringstream statusLocation;
        statusLocation << beamLocation.str() << ASCAN_STATUS_DATASET;
        dsetId = H5Dopen(fileId_, statusLocation.str().c_str(), H5P_DEFAULT);
        datasets.push_back(std::make_shared<AscanStatusBeamDataset>(dsetId, dataLocation.str(), beamIdx));
      }
    }

    return datasets;
  }

  void GetAscanData(const fs::path& filePath_, size_t configId_, const std::string& configName_, ReadOnlyDatasets& dataOut_) const
  {
    std::stringstream dataLocation;
    dataLocation << GROUP_DATA << "/" << configName_ << "/";

    hid_t fileId = m_h5FileMap.at(filePath_);
    hid_t configGroupId = H5Gopen(fileId, dataLocation.str().c_str(), H5P_DEFAULT);
    if (configGroupId >= 0)
    {
      hid_t fileId = m_h5FileMap.at(filePath_);
      std::wstring configName(configName_.begin(), configName_.end());
      herr_t status = H5Gget_objinfo(configGroupId, ASCAN_DATASET, 0, nullptr);
      if (status == 0)
      {
        const auto ascanDatasets = GetAscanMergedBeamDatasets(fileId, dataLocation.str());
        AscanDataSource ascanDataSource(filePath_, configId_, configName);
        dataOut_.Add(std::make_unique<AscanData>(ascanDataSource, ascanDatasets));
      }
      else
      {
        hsize_t beamQty{};
        if (H5Gget_num_objs(configGroupId, &beamQty) >= 0)
        {
          const auto ascanDatasets = GetAscanBeamDatasets(fileId, dataLocation.str(), beamQty);
          AscanDataSource ascanDataSource(filePath_, configId_, configName);
          dataOut_.Add(std::make_unique<AscanData>(ascanDataSource, ascanDatasets));
        }
      }

      H5Gclose(configGroupId);
    }
  }

  TDataMap m_datasetsMap;

  using TH5FileMap = std::map<const fs::path, const hid_t>;
  TH5FileMap m_h5FileMap;

};
