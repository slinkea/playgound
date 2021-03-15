#pragma once
#include <hdf5/hdf5.h>
#include "CudaKernels.cuh"


constexpr char FILENAME_BIG[] = "big.h5";
constexpr char FILENAME_SLICE[] = "slice.h5";

class FileStorage
{
public:
  FileStorage() = default;
  ~FileStorage() = default;
  
  void Read(CudaKernel& ck)
  {
    std::cout << "Opening file..." << std::endl;
    hid_t fileId = H5Fopen(FILENAME_BIG, H5F_ACC_RDONLY, H5P_DEFAULT);
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

  void ReadSlice()
  {
    std::cout << "Opening file..." << std::endl;
    hid_t fileId = H5Fopen(FILENAME_SLICE, H5F_ACC_RDONLY, H5P_DEFAULT);
    hid_t dsetId = H5Dopen(fileId, "DS1", H5P_DEFAULT);

    hsize_t count[3] = { 1, 20, 30 };
    hsize_t offset[3] = { 0, 0, 0 };
    hid_t dataSpaceId = H5Dget_space(dsetId);
    herr_t status = H5Sselect_hyperslab(dataSpaceId, H5S_SELECT_SET, offset, NULL, count, NULL);

    hsize_t dimsm[2] = { 20, 30 };
    hid_t memSpaceId = H5Screate_simple(2, dimsm, NULL);

    std::vector<short> dataSlice(600, 0);

    std::cout << "Reading dataset..." << std::endl;
    status = H5Dread(dsetId, H5T_NATIVE_SHORT, memSpaceId, dataSpaceId, H5P_DEFAULT, dataSlice.data());

    std::cout << "Closing file..." << std::endl;
    status = H5Dclose(dsetId);
    status = H5Fclose(fileId);
  }

  void WriteOneGB()
  {
    hid_t fileId = H5Fcreate(FILENAME_BIG, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    hsize_t dims[3] = { 256, 512, 4096 }; // Scan, Index, Ultrasound
    hid_t spaceId = H5Screate_simple(3, dims, NULL);
    hid_t dsetId = H5Dcreate(fileId, "DS1", H5T_STD_I16LE, spaceId, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    H5D_space_status_t spaceStatus;
    herr_t status = H5Dget_space_status(dsetId, &spaceStatus);

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

    const uint16_t height(10);
    const uint16_t width(20);
    const uint16_t depth(30);
    const hsize_t dims[3] = { height, width, depth };
    hid_t spaceId = H5Screate_simple(3, dims, NULL);
    hid_t dsetId = H5Dcreate(fileId, "DS1", H5T_STD_I16LE, spaceId, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    H5D_space_status_t spaceStatus;
    herr_t status = H5Dget_space_status(dsetId, &spaceStatus);

    std::vector<short> ascanData(depth);
    std::vector<short> dataSource(6000, 0);

    size_t memOffet{};
    for (uint16_t y{}; y < height; y++) {
      for (uint16_t x{}; x < width; x++)
      {
        uint16_t xy = width * y + x;
        std::iota(std::begin(ascanData), std::end(ascanData), xy);

        size_t ascanSize = ascanData.size();
        memcpy(dataSource.data() + memOffet, ascanData.data(), ascanSize * sizeof(uint16_t));
        memOffet += ascanSize;
      }
    }

    status = H5Dwrite(dsetId, H5T_NATIVE_SHORT, H5S_ALL, H5S_ALL, H5P_DEFAULT, dataSource.data());

    status = H5Dget_space_status(dsetId, &spaceStatus);
    hsize_t storageSize = H5Dget_storage_size(dsetId);

    status = H5Dclose(dsetId);
    status = H5Sclose(spaceId);
    status = H5Fclose(fileId);
  }
};
