#include "HDF5AscanTest.h"

#include <chrono>
#include <numeric>

#include <hdf5/hdf5.h>


HDF5AscanTest::HDF5AscanTest()
{
}

HDF5AscanTest::~HDF5AscanTest()
{
}

void HDF5AscanTest::SetUp()
{
}

void HDF5AscanTest::TearDown()
{
}

TEST_F(HDF5AscanTest, WriteBlock)
{
  auto file = H5Fcreate("..\\WriteBlockAscans.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  std::vector<hsize_t> memSize{ 699, 200, 6664 };
  auto memspace = H5Screate_simple((int)memSize.size(), memSize.data(), nullptr);

  std::vector<hsize_t> cdims{ 24, 1, 6664 };
  auto plist = H5Pcreate(H5P_DATASET_CREATE);
  auto errorCode = H5Pset_chunk(plist, (int)cdims.size(), cdims.data());

  auto dataset = H5Dcreate(file, "AscanData", H5T_NATIVE_UCHAR, memspace, H5P_DEFAULT, plist, H5P_DEFAULT);
  auto filespace = H5Dget_space(dataset);

  size_t size{};
  std::vector<uint8_t> ascan(49 * 6664, 1);
  std::vector<std::size_t> offset{ 0, 0, 0 };
  std::vector<std::size_t> count{ 49, 1, 6664 };

  memspace = H5Screate_simple((int)count.size(), count.data(), nullptr);

  auto timingbegin = std::chrono::high_resolution_clock::now();
  
  for (size_t row{}; row < 200; row++)
  {
    offset[0] = row;

    errorCode = H5Sselect_hyperslab(filespace, H5S_SELECT_SET, offset.data(), nullptr, count.data(), nullptr);
    if (errorCode == 0)
    {
      errorCode = H5Dwrite(dataset, H5T_NATIVE_UCHAR, memspace, filespace, H5P_DEFAULT, ascan.data());
      if (errorCode == 0) {
        size += ascan.size();
      }
    }
  }

  auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timingbegin);
  std::cout << "Write A-scan " << size / 1048576.0 << "MBytes: " << duration1.count() << " milliseconds" << std::endl;
}

TEST_F(HDF5AscanTest, WriteBlocks)
{
  auto file = H5Fcreate("..\\WriteBlocksAscans.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  std::vector<hsize_t> memSize{ 699, 200, 6664 };
  auto memspace = H5Screate_simple((int)memSize.size(), memSize.data(), nullptr);

  std::vector<hsize_t> cdims{ 24, 1, 6664 };
  auto plist = H5Pcreate(H5P_DATASET_CREATE);
  auto errorCode = H5Pset_chunk(plist, (int)cdims.size(), cdims.data());

  auto dataset = H5Dcreate(file, "AscanData", H5T_NATIVE_UCHAR, memspace, H5P_DEFAULT, plist, H5P_DEFAULT);
  auto filespace = H5Dget_space(dataset);

  size_t size{};
  std::vector<uint8_t> ascan(49 * 6664, 1);
  std::vector<std::size_t> offset{ 0, 0, 0 };
  std::vector<std::size_t> count{ 49, 1, 6664 };

  memspace = H5Screate_simple((int)count.size(), count.data(), nullptr);

  auto timingbegin = std::chrono::high_resolution_clock::now();

  for (size_t col{}; col < 686; col+=49) {
    for (size_t row{}; row < 200; row++)
    {
      offset[0] = col;
      offset[1] = row;

      errorCode = H5Sselect_hyperslab(filespace, H5S_SELECT_SET, offset.data(), nullptr, count.data(), nullptr);
      if (errorCode == 0)
      {
        errorCode = H5Dwrite(dataset, H5T_NATIVE_UCHAR, memspace, filespace, H5P_DEFAULT, ascan.data());
        if (errorCode == 0) {
          size += ascan.size();
        }
      }
    }
  }

  auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timingbegin);
  std::cout << "Write A-scan " << size / 1048576.0 << "MBytes: " << duration1.count() << " milliseconds" << std::endl;
}

TEST_F(HDF5AscanTest, CreateLargeChunked)
{
  auto file = H5Fcreate("..\\CreateChunkedAscans.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  std::vector<hsize_t> memSize{ 699, 200, 6664 };
  auto memspace = H5Screate_simple((int)memSize.size(), memSize.data(), nullptr);

  std::vector<hsize_t> cdims{ 24, 1, 6664 };
  auto plist = H5Pcreate(H5P_DATASET_CREATE);
  auto errorCode = H5Pset_chunk(plist, (int)cdims.size(), cdims.data());

  auto dataset = H5Dcreate(file, "AscanData", H5T_NATIVE_UCHAR, memspace, H5P_DEFAULT, plist, H5P_DEFAULT);
  auto filespace = H5Dget_space(dataset);


  size_t size{};
  std::vector<uint8_t> ascan(49 * 6664, 1);
  std::vector<std::size_t> offset{ 0, 0, 0 };
  std::vector<std::size_t> count{ 49, 1, 6664 };

  memspace = H5Screate_simple((int)count.size(), count.data(), nullptr);

  auto timingbegin = std::chrono::high_resolution_clock::now();

  for (size_t col{}; col < 15; col++)
  {
    for (size_t row{}; row < 188; row++)
    {
      offset[1] = row;

      errorCode = H5Sselect_hyperslab(filespace, H5S_SELECT_SET, offset.data(), nullptr, count.data(), nullptr);
      if (errorCode == 0)
      {
        errorCode = H5Dwrite(dataset, H5T_NATIVE_UCHAR, memspace, filespace, H5P_DEFAULT, ascan.data());
        if (errorCode == 0) {
          size += ascan.size();
        }
      }
    }
    offset[0] += 44;
  }

  auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timingbegin);
  std::cout << "Write A-scan: " << duration1.count() << " milliseconds" << std::endl;

  errorCode = H5Fflush(file, H5F_SCOPE_LOCAL);

  //

  offset = std::vector<std::size_t>{ 0, 0, 0 };
  ascan = std::vector<uint8_t>(699 * 200 * 6664, 0);
  count = std::vector<std::size_t>{ 699, 200, 6664 };

  memspace = H5Screate_simple((int)count.size(), count.data(), nullptr);

  errorCode = H5Sselect_hyperslab(filespace, H5S_SELECT_SET, offset.data(), nullptr, count.data(), nullptr);
  if (errorCode == 0)
  {
    timingbegin = std::chrono::high_resolution_clock::now();

    errorCode = H5Dread(dataset, H5T_NATIVE_UCHAR, memspace, filespace, H5P_DEFAULT, ascan.data());
    if (errorCode != 0) {
      errorCode = 0;
    }

    auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timingbegin);
    std::cout << "Read A-scan " << size / 1048576.0 << "MBytes: " << duration1.count() << " milliseconds" << std::endl;
  }

  //

  ascan = std::vector<uint8_t>(200 * 6664, 0);
  offset = std::vector<std::size_t>{ 0, 0, 0 };
  count = std::vector<std::size_t>{ 1, 200, 6664 };
  memspace = H5Screate_simple((int)count.size(), count.data(), nullptr);

  timingbegin = std::chrono::high_resolution_clock::now();

  size = 0;
  for (size_t col{}; col < 699; col++)
  {
    offset[0] = col;

    auto errorcode = H5Sselect_hyperslab(filespace, H5S_SELECT_SET, offset.data(), nullptr, count.data(), nullptr);
    if (errorcode == 0)
    {
      errorcode = H5Dread(dataset, H5T_NATIVE_UCHAR, memspace, filespace, H5P_DEFAULT, ascan.data());
      if (errorcode == 0) {
        size += ascan.size();
      }

    }
  }

  auto duration3 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timingbegin);
  std::cout << "Read A-scan " << size / 1048576.0 << "MBytes: " << duration3.count() << " milliseconds" << std::endl;

  //

  ascan = std::vector<uint8_t>(699 * 6664, 0);
  offset = std::vector<std::size_t>{ 0, 0, 0 };
  count = std::vector<std::size_t>{ 699, 1, 6664 };
  memspace = H5Screate_simple((int)count.size(), count.data(), nullptr);

  timingbegin = std::chrono::high_resolution_clock::now();

  size = 0;
  for (size_t col{}; col < 200; col++)
  {
    offset[1] = col;

    auto errorcode = H5Sselect_hyperslab(filespace, H5S_SELECT_SET, offset.data(), nullptr, count.data(), nullptr);
    if (errorcode == 0)
    {
      errorcode = H5Dread(dataset, H5T_NATIVE_UCHAR, memspace, filespace, H5P_DEFAULT, ascan.data());
      if (errorcode == 0) {
        size += ascan.size();
      }

    }
  }

  auto duration4 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timingbegin);
  std::cout << "Read A-scan " << size / 1048576.0 << "MBytes: " << duration4.count() << " milliseconds" << std::endl;

  H5Dclose(dataset);
  H5Fclose(file);
}

TEST_F(HDF5AscanTest, WriteMultipleDataset)
{
  auto file = H5Fcreate("..\\WriteMultipleDataset.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  std::vector<hsize_t> memSize{ 25 * 1024, 1, 5000}; // 25 x 1024 x 5000 x 2 = 1 0240 000
  auto memspace = H5Screate_simple((int)memSize.size(), memSize.data(), nullptr);

  std::vector<hsize_t> cdims{ 105, 1, 5000 }; //1048576 / 5000 / 2 = 104.8576
  auto plist = H5Pcreate(H5P_DATASET_CREATE);
  auto errorCode = H5Pset_chunk(plist, (int)cdims.size(), cdims.data());

  struct Dset
  {
    hid_t dsetId;
    hid_t filesapceId;
  };

  std::vector<Dset> dsets;
  for (size_t dsetIdx{}; dsetIdx < 100; dsetIdx++)
  {
    std::string dsetName("AscanData_" /*+ std::to_string(dsetIdx)*/);
    auto dataset = H5Dcreate(file, dsetName.c_str(), H5T_NATIVE_USHORT, memspace, H5P_DEFAULT, plist, H5P_DEFAULT);
    auto filespace = H5Dget_space(dataset);
    dsets.emplace_back(dataset, filespace);
  }
  
  std::vector<uint16_t> ascan(5000, 1);
  std::iota(ascan.begin(), ascan.end(), 0);

  std::vector<std::size_t> offset{ 0, 0, 0 };
  std::vector<std::size_t> count{ 1, 1, 5000 };
  memspace = H5Screate_simple((int)count.size(), count.data(), nullptr);

  auto timingbegin = std::chrono::high_resolution_clock::now();

  size_t writePoints{};
  for (const auto& dset : dsets)
  {
    for (size_t col{}; col < 1024; col++) //25600
    {
      offset[0] = col;

      errorCode = H5Sselect_hyperslab(dset.filesapceId, H5S_SELECT_SET, offset.data(), nullptr, count.data(), nullptr);
      if (errorCode == 0)
      {
        errorCode = H5Dwrite(dset.dsetId, H5T_NATIVE_USHORT, memspace, dset.filesapceId, H5P_DEFAULT, ascan.data());
        if (errorCode == 0) {
          writePoints += ascan.size();
        }
      }
    }
  }

  auto writeDuration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timingbegin);

  size_t writeSize = writePoints * 2; // H5T_NATIVE_USHORT
  double mib = writeSize / 1048576.0;
  double mibs = mib / writeDuration.count() * 1000.0;
  std::cout << "Write A-scan: " << mib << " MBytes, " << mibs << " MiB/s, " << writeDuration << " milliseconds" << std::endl;
}