#pragma once
#include <string>
#include <vector>
#include <hdf5/hdf5.h>
#include "datasets/DataSets.hpp"
#include "datasets/AscanMergedBeamDataSet.hpp"


constexpr char GROUP_DATA[] = "/Data";
constexpr char BEAM_ONE[] = "Beam 1";
constexpr size_t MAX_NAME_LENGTH = 1024;

class AcquiredData
{
public:
  AcquiredData(const std::string& filePath_)
  {
    herr_t status = H5Eset_auto2(H5E_DEFAULT, NULL, NULL);
    hid_t fileId = H5Fopen(filePath_.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    hid_t dataGroupId = H5Gopen(fileId, GROUP_DATA, H5P_DEFAULT);

    DataSets dataSets;
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
                dataSets.Add(std::move(std::make_unique<AscanMergedBeamDataSet>(configGroupId, configName)));
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
