#pragma once
#include <map>
#include <regex>
#include <vector>
#include <string>
#include <sstream>
#include <filesystem>
#include <hdf5/hdf5.h>
#include "datasets/DataContainer.hpp"
#include "datasets/ascans/AscanData.hpp"

#include "datasets/DataContainer2.hpp"
#include "datasets/ascans/AscanData2.hpp"


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
  AcquiredData()
  {
    //AscanDataSource2 adsrc2(fs::path(), 0, L"");

    //DatasetContainer2 dsc2;
    //dsc2.Add(std::make_unique<AscanDataset2>(1, "A"));
    //dsc2.Add(std::make_unique<AscanBeamDataset2>(2, "B", 0));

    //DataContainer2 dc2("");
    //dc2.Add(std::make_unique<AscanData2>(adsrc2, std::move(dsc2)));
    //const auto& dataItem = dc2.Items()[0];
    //const auto& src22 = dataItem->Source();
    //const auto& datasetItem = dataItem->Datasets();
    //
    //const auto& ds1 = datasetItem.Items()[0];
    //auto ads1 = dynamic_cast<const AscanDataset2*>(ds1.get());
    //auto abds1 = dynamic_cast<const AscanBeamDataset2*>(ds1.get());
    //const auto& loc = ads1->Location();

    //const auto& ds2 = datasetItem.Items()[1];
    //auto ads2 = dynamic_cast<const AscanDataset2*>(ds2.get());
    //auto abds2 = dynamic_cast<const AscanBeamDataset2*>(ds2.get());
    //const auto& dims = abds2->Dimensions();
    //size_t x = dims.X;

    //const auto& attr = abds2->Attributes();
    //const auto& props = abds2->Properties();

    //ads2->SelectSingle(0, 0);

    //std::vector<int16_t> singleAscan(10, 0);
    //ads2->Read(singleAscan.data());
  }

  ~AcquiredData() = default;

  void Open(const fs::path& filePath_)
  {
    H5_RESULT_CHECK(H5Eset_auto(H5E_DEFAULT, NULL, NULL));
    hid_t fileId = H5Fopen(filePath_.string().c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    m_h5FileMap.emplace(filePath_, fileId);

    const auto fileVersion = GetFileVersion(filePath_);
    DataContainer2 dataContainer(fileVersion);

    if (IsEqualOrLess(fileVersion, FILE_VERSION_1_2_0)) {
      FetchData_120(filePath_, std::move(dataContainer));
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

  const DataContainer2& DataContainer(const fs::path& filePath_) const {
    return m_datacontainers.at(filePath_);
  }

  DataContainer2& DataContainer(const fs::path& filePath_) {
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

  void FetchData_120(const fs::path& filePath_, DataContainer2&& dataContainer)
  {
    hsize_t groupQty{};
    char name[MAX_NAME_LENGTH];
    hid_t dataGroupId = H5Gopen(m_h5FileMap[filePath_], GROUP_DATA, H5P_DEFAULT);
    if (H5Gget_num_objs(dataGroupId, &groupQty) >= 0)
    {
      for (hsize_t groupIdx{}; groupIdx < groupQty; groupIdx++)
      {
        ssize_t nameLength = H5Gget_objname_by_idx(dataGroupId, groupIdx, name, MAX_NAME_LENGTH);
        if (nameLength > 0) 
        {
          auto ascanData = FetchAscanData(filePath_, groupIdx, std::string(name));  //[TODO[EAB] Utiliser un id provenant de la config.]
          dataContainer.Add(std::move(ascanData));
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
    ascanData_->Datasets().Add(std::make_unique<AscanDataset2>(dsetId, location_));

    std::stringstream statusLocation;
    statusLocation << location_ << ASCAN_STATUS_DATASET;
    dsetId = H5Dopen(fileId_, statusLocation.str().c_str(), H5P_DEFAULT);
    ascanData_->Datasets().Add(std::make_unique<AscanDataset2>(dsetId, location_));
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
        ascanData_->Datasets().Add(std::make_unique<AscanBeamDataset2>(dsetId, location_, beamIdx));

        std::stringstream statusLocation;
        statusLocation << beamLocation.str() << ASCAN_STATUS_DATASET;
        dsetId = H5Dopen(fileId_, statusLocation.str().c_str(), H5P_DEFAULT);
        ascanData_->Datasets().Add(std::make_unique<AscanBeamDataset2>(dsetId, location_, beamIdx));
      }
    }
  }

  TAscanDataPtr&& FetchAscanData(const fs::path& filePath_, size_t configId_, const std::string& configName_) const
  {
    std::stringstream dataLocation;
    TAscanDataPtr ascanData;
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
        FetchAscanDatasets(fileId, dataLocation.str(), ascanData);
      }
      else
      {
        hsize_t beamQty{};
        if (H5Gget_num_objs(configGroupId, &beamQty) >= 0)
        {
          FetchAscanBeamDatasets(fileId, dataLocation.str(), beamQty, ascanData);
        }
      }

      H5Gclose(configGroupId);
    }

    return std::move(ascanData);
  }

  TDataContainerMap m_datacontainers;

  using TH5FileMap = std::map<const fs::path, const hid_t>;
  TH5FileMap m_h5FileMap;
};
