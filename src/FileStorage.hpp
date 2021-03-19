#pragma once
#include <hdf5/hdf5.h>
#include "CudaKernels.cuh"
#include "datasets/AscanDataSet.hpp"
#include "datasets/AscanBeamDataSet.hpp"
#include "datasets/DataSets.hpp"

constexpr char FILENAME_BIG[] = "big.h5";
constexpr char FILENAME_SLICE[] = "slice.h5";
constexpr char FILENAME_FPD[] = "ThinBlade.h5";


class FileStorage
{
public:
  FileStorage()
  {
    hid_t fileId = H5Fopen(FILENAME_FPD, H5F_ACC_RDONLY, H5P_DEFAULT);
    hid_t dsetId = H5Dopen(fileId, "DS1", H5P_DEFAULT);

    IDataSet* x = new AscanDataSet(dsetId);
    IDataSet* y = new AscanBeamDataSet(dsetId, 0);

    auto ascanDataSet = dynamic_cast<IAscanDataSet*>(x);
    
    auto ascanBeamDataSet = dynamic_cast<IAscanBeamDataSet*>(x);
    
    auto ascanDataSet2 = dynamic_cast<IAscanDataSet*>(y);
    auto ascanBeamDataSet2 = dynamic_cast<IAscanBeamDataSet*>(y);
    

    DataSets dataSets;
    dataSets.Add(std::move(std::make_unique<AscanDataSet>(dsetId)));
    dataSets.Add(std::move(std::make_unique<AscanBeamDataSet>(dsetId, 0)));

    //AscanDataSet ads(dsetId);
    //AscanBeamDataSet abds;
  }

  ~FileStorage() = default;
  
  void Read(CudaKernel& ck)
  {
    std::cout << "Opening file..." << std::endl;
    hid_t fileId = H5Fopen(FILENAME_FPD, H5F_ACC_RDONLY, H5P_DEFAULT);
    hid_t dsetId = H5Dopen(fileId, "DS1", H5P_DEFAULT);

    std::cout << "Allocating memory..." << std::endl;
    ck.MemoryAllocation();

    std::cout << "Reading dataset..." << std::endl;
    herr_t status = H5Dread(dsetId, H5T_NATIVE_SHORT, H5S_ALL, H5S_ALL, H5P_DEFAULT, ck.HostMemory());

    ck.CopyMemoryToDevice();
    ck.Process();

    std::cout << "Closing file..." << std::endl;
    status = H5Dclose(dsetId);
    status = H5Fclose(fileId);
  }

  std::vector<int8_t> ReadSliceFpd()
  {
    std::cout << "Opening file..." << std::endl;
    hid_t fileId = H5Fopen(FILENAME_FPD, H5F_ACC_RDONLY, H5P_DEFAULT);
    hid_t dsetId = H5Dopen(fileId, "/Data/Default PA/Ascan Data", H5P_DEFAULT);

    const hsize_t width(200);
    const hsize_t depth(6664);

    hsize_t count[3] = { 1, width, depth };
    hsize_t offset[3] = { 0, 0, 0 };
    hid_t dataSpaceId = H5Dget_space(dsetId);
    herr_t status = H5Sselect_hyperslab(dataSpaceId, H5S_SELECT_SET, offset, NULL, count, NULL);

    hsize_t dimsm[2] = { width, depth };
    hid_t memSpaceId = H5Screate_simple(2, dimsm, NULL);

    std::vector<int8_t> dataSlice(width * depth, 0);

    std::cout << "Reading dataset..." << std::endl;
    status = H5Dread(dsetId, H5T_NATIVE_CHAR, memSpaceId, dataSpaceId, H5P_DEFAULT, dataSlice.data());

    std::cout << "Closing file..." << std::endl;
    status = H5Dclose(dsetId);
    status = H5Fclose(fileId);

    return dataSlice;
  }

  std::vector<uint16_t> ReadSlice()
  {
    

    std::cout << "Opening file..." << std::endl;
    hid_t fileId = H5Fopen(FILENAME_SLICE, H5F_ACC_RDONLY, H5P_DEFAULT);
    hid_t dsetId = H5Dopen(fileId, "DS1", H5P_DEFAULT);

    const hsize_t height(100);
    const hsize_t depth(256);

    hsize_t count[3] = { 1, height, depth };
    hsize_t offset[3] = { 0, 0, 0 };
    hid_t dataSpaceId = H5Dget_space(dsetId);
    herr_t status = H5Sselect_hyperslab(dataSpaceId, H5S_SELECT_SET, offset, NULL, count, NULL);

    hsize_t dimsm[2] = { height, depth };
    hid_t memSpaceId = H5Screate_simple(2, dimsm, NULL);

    std::vector<uint16_t> dataSlice(height * depth, 0);

    std::cout << "Reading dataset..." << std::endl;
    status = H5Dread(dsetId, H5T_NATIVE_USHORT, memSpaceId, dataSpaceId, H5P_DEFAULT, dataSlice.data());

    std::cout << "Closing file..." << std::endl;
    status = H5Dclose(dsetId);
    status = H5Fclose(fileId);

    return dataSlice;
  }

  void WriteOneGB()
  {
    hid_t fileId = H5Fcreate(FILENAME_BIG, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    hsize_t dims[3] = { 256, 512, 4096 }; // Scan, Index, Ultrasound

    hsize_t  cdims[3];
    cdims[0] = 1;
    cdims[1] = 512;
    cdims[2] = 4096;
    hid_t plist_id = H5Pcreate(H5P_DATASET_CREATE);
    herr_t status = H5Pset_chunk(plist_id, 3, cdims);
    status = H5Pset_deflate(plist_id, 9);

    hid_t spaceId = H5Screate_simple(3, dims, NULL);
    hid_t dsetId = H5Dcreate(fileId, "DS1", H5T_STD_I16LE, spaceId, H5P_DEFAULT, plist_id, H5P_DEFAULT);

    H5D_space_status_t spaceStatus;
    status = H5Dget_space_status(dsetId, &spaceStatus);

    std::vector<short> wdata((short)1 << 29, 1);
    //memset(wdata, 0, sizeof(wdata[0][0][0]) * 512 * 1024 * 2048);

    status = H5Dwrite(dsetId, H5T_NATIVE_SHORT, H5S_ALL, H5S_ALL, H5P_DEFAULT, wdata.data());

    status = H5Dget_space_status(dsetId, &spaceStatus);
    hsize_t storageSize = H5Dget_storage_size(dsetId);

    status = H5Dclose(dsetId);
    status = H5Sclose(spaceId);
    status = H5Fclose(fileId);
  }

  void Write()
  {
    hid_t fileId = H5Fcreate(FILENAME_SLICE, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    const hsize_t height(100);
    const hsize_t width(200);
    const hsize_t depth(256);
    const hsize_t dims[3] = { height, width, depth };
    hid_t spaceId = H5Screate_simple(3, dims, NULL);
    hid_t dsetId = H5Dcreate(fileId, "DS1", H5T_NATIVE_USHORT, spaceId, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    H5D_space_status_t spaceStatus;
    herr_t status = H5Dget_space_status(dsetId, &spaceStatus);

    std::vector<uint16_t> ascanData(depth);
    std::vector<uint16_t> dataSource(height * width * depth, 0);

    size_t memOffet{};
    for (hsize_t y{}; y < height; y++) {
      for (hsize_t x{}; x < width; x++)
      {
        //hsize_t xy = width * y + x;

        for (hsize_t z{}; z < depth / 2; z++) {
          ascanData[z] = static_cast<uint16_t>(z * 512);
        }

        for (hsize_t z(depth / 2); z < depth; z++) {
          ascanData[z] = static_cast<uint16_t>(65535 - z * 512);
        }

        size_t ascanSize = ascanData.size();
        memcpy(dataSource.data() + memOffet, ascanData.data(), ascanSize * sizeof(uint16_t));
        memOffet += ascanSize;
      }
    }

    status = H5Dwrite(dsetId, H5T_NATIVE_USHORT, H5S_ALL, H5S_ALL, H5P_DEFAULT, dataSource.data());

    status = H5Dget_space_status(dsetId, &spaceStatus);
    hsize_t storageSize = H5Dget_storage_size(dsetId);

    status = H5Dclose(dsetId);
    status = H5Sclose(spaceId);
    status = H5Fclose(fileId);
  }

  void CompressFile()
  {
    hid_t fileId = H5Fopen(FILENAME_FPD, H5F_ACC_RDONLY, H5P_DEFAULT);
    hid_t dsetId = H5Dopen(fileId, "/Data/Default PA/Ascan Data", H5P_DEFAULT);

    const hsize_t height(699);
    const hsize_t width(200);
    const hsize_t depth(6664);

    hid_t dataSpaceId = H5Dget_space(dsetId);

    hsize_t dimsm[3] = { height, width, depth };
    hid_t memSpaceId = H5Screate_simple(3, dimsm, NULL);

    std::vector<int8_t> dataFile(height * width * depth, 0);

    herr_t status = H5Dread(dsetId, H5T_NATIVE_CHAR, memSpaceId, dataSpaceId, H5P_DEFAULT, dataFile.data());

    status = H5Sclose(memSpaceId);
    status = H5Sclose(dataSpaceId);
    status = H5Dclose(dsetId);
    status = H5Fclose(fileId);

    //

    fileId = H5Fcreate(FILENAME_BIG, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    hsize_t  cdims[3];
    cdims[0] = 1;
    cdims[1] = 200;
    cdims[2] = 6664;
    hid_t plist_id = H5Pcreate(H5P_DATASET_CREATE);
    status = H5Pset_chunk(plist_id, 3, cdims);
    status = H5Pset_deflate(plist_id, 9);

    hid_t spaceId = H5Screate_simple(3, dimsm, NULL);
    dsetId = H5Dcreate(fileId, "DS1", H5T_NATIVE_CHAR, spaceId, H5P_DEFAULT, plist_id, H5P_DEFAULT);

    H5D_space_status_t spaceStatus;
    status = H5Dget_space_status(dsetId, &spaceStatus);

    status = H5Dwrite(dsetId, H5T_NATIVE_CHAR, H5S_ALL, H5S_ALL, H5P_DEFAULT, dataFile.data());

    status = H5Dget_space_status(dsetId, &spaceStatus);
    hsize_t storageSize = H5Dget_storage_size(dsetId);

    status = H5Dclose(dsetId);
    status = H5Sclose(spaceId);
    status = H5Fclose(fileId);
  }
};
