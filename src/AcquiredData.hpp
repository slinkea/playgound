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
  using TDatasets = std::vector<std::shared_ptr<const IDataset>>;

  AcquiredData(const fs::path& filePath_)
  {
    H5_RESULT_CHECK(H5Eset_auto(H5E_DEFAULT, NULL, NULL));
    m_fileId = H5Fopen(filePath_.string().c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);

    const auto fileVersion = GetFileVersion();
    if (IsEqualOrLess(fileVersion, FILE_VERSION_1_2_0))
    {
      FetchAscanData120(filePath_);
    }
  }

  ~AcquiredData()
  {
    herr_t status = H5Fclose(m_fileId);
  }

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

  const std::string GetFileVersion() const
  {
    char fileVersion[MAX_NAME_LENGTH]{};
    hid_t attrId = H5Aopen(m_fileId, FLE_VERSION, H5P_DEFAULT);
    hid_t attrType = H5Aget_type(attrId);
    H5_RESULT_CHECK(H5Aread(attrId, attrType, fileVersion));
    H5Aclose(attrId);

    return std::string(fileVersion);
  }

  void FetchAscanData120(const fs::path& filePath_)
  {
    hsize_t groupQty{};
    char name[MAX_NAME_LENGTH];
    hid_t dataGroupId = H5Gopen(m_fileId, GROUP_DATA, H5P_DEFAULT);
    if (H5Gget_num_objs(dataGroupId, &groupQty) >= 0)
    {
      ONDTLib::Container<IReadOnlyData> datasets;
      for (hsize_t groupIdx{}; groupIdx < groupQty; groupIdx++)
      {
        ssize_t nameLength = H5Gget_objname_by_idx(dataGroupId, groupIdx, name, MAX_NAME_LENGTH);
        if (nameLength > 0) {
          GetAscanData(filePath_, std::string(name), datasets);
        }
      }

      m_datasetsDic.emplace(std::make_pair(filePath_, std::move(datasets)));
    }

    H5Gclose(dataGroupId);
  }

  const TDatasets GetAscanMergedBeamDatasets(const std::string& location_) const
  {
    TDatasets datasets;

    std::stringstream dataLocation;
    dataLocation << location_ << ASCAN_DATASET;
    hid_t dsetId = H5Dopen(m_fileId, dataLocation.str().c_str(), H5P_DEFAULT);
    datasets.push_back(std::make_shared<AscanMergedBeamDataset>(dsetId, dataLocation.str()));

    std::stringstream statusLocation;
    statusLocation << location_ << ASCAN_STATUS_DATASET;
    dsetId = H5Dopen(m_fileId, statusLocation.str().c_str(), H5P_DEFAULT);
    datasets.push_back(std::make_shared<AscanStatusMergedBeamDataset>(dsetId, dataLocation.str()));

    return datasets;
  }

  const TDatasets GetAscanBeamDatasets(const std::string& location_, hsize_t beamQty_) const
  {
    TDatasets datasets;

    for (size_t beamIdx{}; beamIdx < beamQty_; beamIdx++)
    {
      std::stringstream beamLocation;
      std::string beamStr(BEAM_PREFIX + std::string(" ") + std::to_string(beamIdx + 1));
      beamLocation << location_ << beamStr << "/";

      herr_t status = H5Gget_objinfo(m_fileId, beamLocation.str().c_str(), 0, nullptr);
      if (status == 0)
      {
        std::stringstream dataLocation;
        dataLocation << beamLocation.str() << ASCAN_DATASET;
        hid_t dsetId = H5Dopen(m_fileId, dataLocation.str().c_str(), H5P_DEFAULT);
        datasets.push_back(std::make_shared<AscanBeamDataset>(dsetId, dataLocation.str(), beamIdx));

        std::stringstream statusLocation;
        statusLocation << beamLocation.str() << ASCAN_STATUS_DATASET;
        dsetId = H5Dopen(m_fileId, statusLocation.str().c_str(), H5P_DEFAULT);
        datasets.push_back(std::make_shared<AscanStatusBeamDataset>(dsetId, dataLocation.str(), beamIdx));
      }
    }

    return datasets;
  }

  void GetAscanData(const fs::path& filePath_, const std::string& configName_, ONDTLib::Container<IReadOnlyData>& dataOut_) const
  {
    std::stringstream dataLocation;
    dataLocation << GROUP_DATA << "/" << configName_ << "/";

    hid_t configGroupId = H5Gopen(m_fileId, dataLocation.str().c_str(), H5P_DEFAULT);
    if (configGroupId >= 0)
    {
      std::wstring configName(configName_.begin(), configName_.end());
      herr_t status = H5Gget_objinfo(configGroupId, ASCAN_DATASET, 0, nullptr);
      if (status == 0)
      {
        const auto ascanDatasets = GetAscanMergedBeamDatasets(dataLocation.str());
        AscanDataSource ascanDataSource(filePath_, 0, configName, ascanDatasets); //[TODO[EAB] Utiliser un id provenant de la config.]
        dataOut_.Add(std::make_unique<AscanData>(ascanDataSource));
      }
      else
      {
        hsize_t beamQty{};
        if (H5Gget_num_objs(configGroupId, &beamQty) >= 0)
        {
          const auto ascanDatasets = GetAscanBeamDatasets(dataLocation.str(), beamQty);
          AscanDataSource ascanDataSource(filePath_, 0, configName, ascanDatasets); //[TODO[EAB] Utiliser un id provenant de la config.]
          dataOut_.Add(std::make_unique<AscanData>(ascanDataSource));
        }
      }

      H5Gclose(configGroupId);
    }
  }

private:
  hid_t m_fileId{};
  using TDataDictionnary = std::map<const fs::path, ONDTLib::Container<IReadOnlyData>>;
  TDataDictionnary m_datasetsDic;
};
