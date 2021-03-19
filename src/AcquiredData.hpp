#pragma once
#include <string>
#include <vector>
#include <hdf5/hdf5.h>
#include "datasets/DataSets.hpp"
#include "datasets/AscanMergedBeamDataSet.hpp"


constexpr char FILENAME[] = "ThinBlade.h5";
constexpr char GROUP_DATA[] = "/Data";
constexpr char ASCAN_DATASET[] = "Ascan Data";
constexpr char BEAM_ONE[] = "Beam 1";
constexpr size_t MAX_NAME_LENGTH = 1024;

class AcquiredData
{
public:
  AcquiredData()
  {
    DataSets dataSets;

    hid_t fileId = H5Fopen(FILENAME, H5F_ACC_RDONLY, H5P_DEFAULT);
    hid_t dataGroupId = H5Gopen(fileId, GROUP_DATA, H5P_DEFAULT);

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
            herr_t status = H5Gget_objinfo(configGroupId, BEAM_ONE, 0, NULL);
            if (status == 0)
            {

            }
            else
            {
              hid_t dataSetId = H5Dopen(configGroupId, ASCAN_DATASET, H5P_DEFAULT);
              if (dataSetId >= 0)
              {
                std::wstring configName(nameChar.begin(), nameChar.end());
                dataSets.Add(std::move(std::make_unique<AscanMergedBeamDataSet>(dataSetId, configName)));
              }

              H5Dclose(dataSetId);
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
