#pragma once
#include <string>
#include <sstream>
#include <map>
#include <vector>
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

//typedef struct {
//  const char* path;           /* Path to object */
//  H5O_type_t type;            /* Type of object */
//} obj_visit_t;

typedef struct {
  unsigned idx;               /* Index in object visit structure */
  //const obj_visit_t* info;    /* Pointer to the object visit structure to use */
} ovisit_ud_t;

static std::vector<std::string> m_groupPath;
static std::vector<std::string> m_datasetPath;

class AcquiredData
{
public:
  AcquiredData(const fs::path& filePath_)
  {
    //std::vector<const IDataset*> datasets;
    //const IAscanDataset* data = new AscanMergedBeamDataset(0, "./Data/Default PA/Ascan Data");
    //datasets.push_back(data);

    //IAscanDataset* status = new AscanMergedBeamStatusDataset("./Data/Default PA/Ascan Status");
    
    //const IAscanDataset* data2 = new AscanBeamDataset(0, "./Data/Default PA/Beam 1/Ascan Data", 0);
    //datasets.push_back(data2);


    //const AscanDataSource ds(L"Default PA", datasets);
    //const AscanData* ad = new AscanData(ds);
    //m_data.push_back(ad);

    //auto s = ad->Source();
    //s->Name();

    FetchAscanData(filePath_);
    //Fetch2(filePath_);
  }

  const std::vector<std::shared_ptr<const IDataset>> GetAscanDatasets(hid_t fileId_, const std::string& location_)
  {
    std::vector<std::shared_ptr<const IDataset>> datasets;

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

  void FetchAscanData(const fs::path& filePath_)
  {
    herr_t status = H5Eset_auto(H5E_DEFAULT, NULL, NULL);
    hid_t fileId = H5Fopen(filePath_.string().c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    
    char fileVersion[256]{};
    hid_t attrId = H5Aopen(fileId, "File Version", H5P_DEFAULT);
    hid_t attrType = H5Aget_type(attrId);
    status = H5Aread(attrId, attrType, fileVersion);
    
    hid_t dataGroupId = H5Gopen(fileId, GROUP_DATA, H5P_DEFAULT);

    hsize_t groupQty{};
    char name[MAX_NAME_LENGTH];
    if (H5Gget_num_objs(dataGroupId, &groupQty) >= 0)
    {
      ONDTLib::Container<IReadOnlyData> readOnlyDatasets;
      for (hsize_t groupIdx{}; groupIdx < groupQty; groupIdx++)
      {
        ssize_t nameLength = H5Gget_objname_by_idx(dataGroupId, groupIdx, name, MAX_NAME_LENGTH);
        if (nameLength > 0)
        {
          std::vector<char> nameChar(name, name + nameLength);
          std::string configName(nameChar.begin(), nameChar.end());

          hid_t configGroupId = H5Gopen(dataGroupId, configName.c_str(), H5P_DEFAULT);
          if (configGroupId >= 0)
          {
            status = H5Gget_objinfo(configGroupId, ASCAN_DATASET, 0, nullptr);
            if (status == 0)
            {
              std::stringstream dataLocation;
              dataLocation << GROUP_DATA << "/" << configName << "/";
              const auto ascanDatasets = GetAscanDatasets(fileId, dataLocation.str());

              std::wstring configNameWide(configName.begin(), configName.end());
              AscanDataSource ascanDataSource(filePath_, 0, configNameWide, ascanDatasets); //[TODO[EAB] Utiliser un id provenant de la config.]
              readOnlyDatasets.Add(std::make_unique<AscanData>(ascanDataSource));
            }
            else
            {
              std::vector<const IDataset*> datasets;


              hsize_t beamQty{};
              if (H5Gget_num_objs(configGroupId, &beamQty) >= 0)
              {
                for (size_t beamIdx{}; beamIdx < beamQty; beamIdx++)
                {
                  std::string beamStr("Beam " + std::to_string(beamIdx + 1));

                  std::stringstream path;
                  path << beamStr << "/" << ASCAN_DATASET;
                  status = H5Gget_objinfo(configGroupId, path.str().c_str(), 0, nullptr);
                  if (status == 0)
                  {
                    hid_t beamGroupId = H5Gopen(configGroupId, beamStr.c_str(), H5P_DEFAULT);

                    std::stringstream dataBeamLocation;
                    dataBeamLocation << GROUP_DATA << "/" << name << "/" << beamStr << "/";

                    std::stringstream dataLocation;
                    dataLocation << dataBeamLocation.str() << ASCAN_DATASET;
                    hid_t dsetId = H5Dopen(beamGroupId, ASCAN_DATASET, H5P_DEFAULT);
                    const IAscanDataset* ascanDataset = new AscanBeamDataset(dsetId, dataLocation.str(), beamIdx);
                    datasets.push_back(ascanDataset);

                    std::stringstream statusLocation;
                    statusLocation << dataBeamLocation.str() << ASCAN_STATUS_DATASET;
                    dsetId = H5Dopen(beamGroupId, ASCAN_STATUS_DATASET, H5P_DEFAULT);
                    const IAscanDataset* ascanStatusDataset = new AscanStatusBeamDataset(dsetId, statusLocation.str(), beamIdx);
                    datasets.push_back(ascanStatusDataset);
                  }
                }

                std::wstring configNameWide(configName.begin(), configName.end());
                //const AscanDataSource ascanDataSource(filePath_, 0, configNameWide, datasets); //[TODO[EAB] Utiliser un id provenant de la config.]
                //readOnlyDatasets.Add(std::make_unique<AscanData>(ascanDataSource));
              }
            }

            H5Gclose(configGroupId);
          }
        }
      }

      m_readOnlyDatasets.emplace(std::make_pair(filePath_, std::move(readOnlyDatasets)));
    }

    H5Gclose(dataGroupId);
  }

  static int Visit(hid_t id_, const char* name_, const H5O_info_t* oinfo_, void* op_data_)
  {
    ovisit_ud_t* op_data = static_cast <ovisit_ud_t*>(op_data_);

    if (oinfo_->type == H5O_type_t::H5O_TYPE_GROUP)
    {
      m_groupPath.push_back(name_);

      H5O_info_t oinfo;
      H5Oget_info_by_name(id_, name_, &oinfo, H5O_INFO_NUM_ATTRS, H5P_DEFAULT);
    }
    else if (oinfo_->type == H5O_type_t::H5O_TYPE_DATASET)
    {
      m_datasetPath.push_back(name_);

      H5O_info_t oinfo;
      H5Oget_info_by_name(id_, name_, &oinfo, H5O_INFO_NUM_ATTRS, H5P_DEFAULT);

      for (unsigned i = 0; i < (unsigned)oinfo.num_attrs; i++)
      {
        hid_t dsetId = H5Dopen(id_, name_, H5P_DEFAULT);
        hid_t attr = H5Aopen_by_idx(dsetId, ".", H5_INDEX_CRT_ORDER, H5_ITER_INC, (hsize_t)i, H5P_DEFAULT, H5P_DEFAULT);
        hid_t atype = H5Aget_type(attr);
        H5T_class_t type_class = H5Tget_class(atype);
        if (type_class == H5T_STRING) {
          hid_t atype_mem = H5Tget_native_type(atype, H5T_DIR_ASCEND);
          atype_mem = 0;
        }
      }
    }

    op_data->idx++;

    return H5_ITER_CONT;
  }

  void Fetch2(const std::string& filePath_)
  {
    hid_t fileId = H5Fopen(filePath_.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);

    ovisit_ud_t udata;
    H5Ovisit(fileId, H5_INDEX_NAME, H5_ITER_INC, Visit, &udata, H5O_INFO_BASIC);

    size_t x = m_groupPath.size();
    x = m_datasetPath.size();
  }

  ~AcquiredData() = default;

private:
  std::map<const fs::path, ONDTLib::Container<IReadOnlyData>> m_readOnlyDatasets;

};
