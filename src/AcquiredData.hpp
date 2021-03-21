#pragma once
#include <string>
#include <vector>
#include <hdf5/hdf5.h>
#include "datasets/Datasets.hpp"
#include "datasets/AscanMergedBeamDataset.hpp"


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
  AcquiredData(const std::string& filePath_)
  {
    Fetch1(filePath_);
    //Fetch2(filePath_);
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

  void Fetch1(const std::string& filePath_)
  {
    herr_t status = H5Eset_auto(H5E_DEFAULT, NULL, NULL);
    hid_t fileId = H5Fopen(filePath_.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    hid_t dataGroupId = H5Gopen(fileId, GROUP_DATA, H5P_DEFAULT);

    Datasets datasets;
    ssize_t nameLength{};
    char name[MAX_NAME_LENGTH];

    hsize_t groupQty{};
    if (H5Gget_num_objs(dataGroupId, &groupQty) >= 0)
    {
      for (hsize_t groupIdx(0); groupIdx < groupQty; groupIdx++)
      {
        nameLength = H5Gget_objname_by_idx(dataGroupId, groupIdx, name, MAX_NAME_LENGTH);
        if (nameLength > 0)
        {
          std::vector<char> nameChar(name, name + nameLength);
          std::string configName(nameChar.begin(), nameChar.end());

          hid_t configGroupId = H5Gopen(dataGroupId, configName.c_str(), H5P_DEFAULT);
          if (configGroupId >= 0)
          {
            status = H5Gget_objinfo(configGroupId, BEAM_ONE, 0, nullptr);
            if (status == 0)
            {

            }
            else
            {
              status = H5Gget_objinfo(configGroupId, ASCAN_DATASET, 0, nullptr);
              if (status == 0)
              {
                std::wstring configName(nameChar.begin(), nameChar.end());
                hid_t ascanDataId = H5Dopen(configGroupId, ASCAN_DATASET, H5P_DEFAULT);
                hid_t ascanStatusId = H5Dopen(configGroupId, ASCAN_STATUS_DATASET, H5P_DEFAULT);
                datasets.Add(std::move(std::make_unique<AscanMergedBeamDataset>(ascanDataId, ascanStatusId, configName)));
              }
            }

            H5Gclose(configGroupId);
          }
        }
      }
    }

    H5Gclose(dataGroupId);
  }

  ~AcquiredData() = default;
};
