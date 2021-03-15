#pragma once
#include <hdf5/hdf5.h>
#include "CudaKernels.cuh"


constexpr char FILENAME[] = "testfile.h5";

class FileStorage
{
public:
  FileStorage(CudaKernel& ck)
  {
    if (1)
    {
      hid_t fileId = H5Fcreate(FILENAME, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

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

    if (0)
    {
      std::cout << "Opening file..." << std::endl;
      hid_t fileId = H5Fopen(FILENAME, H5F_ACC_RDONLY, H5P_DEFAULT);
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
  }

  ~FileStorage() = default;
  
  
};
