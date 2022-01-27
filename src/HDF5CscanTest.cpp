#include "HDF5CscanTest.h"

#include <chrono>

#include <hdf5/hdf5.h>


HDF5CscanTest::HDF5CscanTest()
{
}

HDF5CscanTest::~HDF5CscanTest()
{
}

void HDF5CscanTest::SetUp()
{
}

void HDF5CscanTest::TearDown()
{
}

void HDF5CscanTest::CreateChunkedCscan()
{
  auto file = H5Fcreate("..\\CreateChunkedCscans.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  std::vector<hsize_t> memSize{ 14250, 862, 5 };
  auto memspace = H5Screate_simple((int)memSize.size(), memSize.data(), nullptr);

  std::vector<hsize_t> cdims{ 57, 57, 1 };
  auto plist = H5Pcreate(H5P_DATASET_CREATE);
  auto errorCode = H5Pset_chunk(plist, (int)cdims.size(), cdims.data());
  //errorCode = H5Pset_deflate(plist, 1);

  const auto dataTypeId1 = H5Tcreate(H5T_COMPOUND, sizeof(CompoundCscanData::RDTIFF19::Rectified));
  H5Tinsert(dataTypeId1, "Amplitude", HOFFSET(CompoundCscanData::RDTIFF19::Rectified, Amplitude), H5T_NATIVE_INT);
  H5Tinsert(dataTypeId1, "Time", HOFFSET(CompoundCscanData::RDTIFF19::Rectified, Time), H5T_NATIVE_USHORT);
  H5Tinsert(dataTypeId1, "Statuses", HOFFSET(CompoundCscanData::RDTIFF19::Rectified, Statuses), H5T_NATIVE_B8);

  //const auto dataTypeId2 = H5Tcreate(H5T_COMPOUND, sizeof(CompoundCscanData::OpenView));
  //H5Tinsert(dataTypeId2, "HasData", HOFFSET(CompoundCscanData::OpenView, HasData), H5T_NATIVE_HBOOL);
  //H5Tinsert(dataTypeId2, "CrossingTime", HOFFSET(CompoundCscanData::OpenView, CrossingTime), H5T_NATIVE_FLOAT);
  //H5Tinsert(dataTypeId2, "AmplitudeTime", HOFFSET(CompoundCscanData::OpenView, PeakTime), H5T_NATIVE_FLOAT);
  //H5Tinsert(dataTypeId2, "Amplitude", HOFFSET(CompoundCscanData::OpenView, Amplitude), H5T_NATIVE_INT);
  //H5Tinsert(dataTypeId2, "Synced", HOFFSET(CompoundCscanData::OpenView, IsSyncedOnSource), H5T_NATIVE_HBOOL);
  //H5Tinsert(dataTypeId2, "Saturated", HOFFSET(CompoundCscanData::OpenView, IsSaturated), H5T_NATIVE_HBOOL);
  //H5Tinsert(dataTypeId2, "Detected", HOFFSET(CompoundCscanData::OpenView, IsDetected), H5T_NATIVE_HBOOL);

  auto dataset = H5Dcreate(file, "CscanData", dataTypeId1, memspace, H5P_DEFAULT, plist, H5P_DEFAULT);

  H5Dclose(dataset);
  H5Fclose(file);
}

void HDF5CscanTest::ReadCscan(size_t gateOffset, std::vector<std::vector<std::vector<CompoundCscanData::OpenView>>>& chunks)
{
  auto file = H5Fopen("..\\DataFile_3.h5", H5F_ACC_RDONLY, H5P_DEFAULT);
  auto dataset = H5Dopen(file, "/Domain/Data/Configurations/0/CscanData", H5P_DEFAULT);
  auto filespace = H5Dget_space(dataset);

  auto rank = H5Sget_simple_extent_ndims(filespace);
  std::vector<hsize_t> dims(rank);
  H5Sget_simple_extent_dims(filespace, dims.data(), nullptr);

  std::vector<hsize_t> memdims{ dims[0], dims[1] };
  auto memspace = H5Screate_simple((int)memdims.size(), memdims.data(), nullptr);
  std::vector<CompoundCscanData::OpenView> dataOut = std::vector<CompoundCscanData::OpenView>(memdims[0] * memdims[1]);

  std::vector<std::size_t> offset{ 0, 0, gateOffset };
  std::vector<std::size_t> count{ memdims[0] , memdims[1], 1 };

  auto errorCode = H5Sselect_hyperslab(filespace, H5S_SELECT_SET, offset.data(), nullptr, count.data(), nullptr);
  if (errorCode != 0) {
    errorCode = 0;
  }

  errorCode = H5Dread(dataset, H5Dget_type(dataset), memspace, filespace, H5P_DEFAULT, dataOut.data());
  if (errorCode != 0) {
    errorCode = 0;
  }

  chunks.resize(14250 / 57);
  std::vector<std::vector<size_t>> countByChunk{ 14250 / 57 };
  for (size_t col{}; col < chunks.size(); col++) {
    chunks[col].resize(16);
    countByChunk[col].resize(16);

    for (size_t row{}; row < chunks[col].size(); row++) {
      chunks[col][row].resize(57 * 57);
    }
  }

  for (size_t cell{}; cell < dataOut.size(); cell++)
  {
    size_t col = cell / 862;
    size_t row = cell % 862;

    size_t chunkCol = col / 57;
    size_t chunkRow = row / 57;

    size_t chunkCell = countByChunk[chunkCol][chunkRow];

    chunks[chunkCol][chunkRow][chunkCell].HasData = dataOut[cell].HasData;
    chunks[chunkCol][chunkRow][chunkCell].CrossingTime = dataOut[cell].CrossingTime;
    chunks[chunkCol][chunkRow][chunkCell].PeakTime = dataOut[cell].PeakTime;
    chunks[chunkCol][chunkRow][chunkCell].Amplitude = dataOut[cell].Amplitude;
    chunks[chunkCol][chunkRow][chunkCell].IsSyncedOnSource = dataOut[cell].IsSyncedOnSource;
    chunks[chunkCol][chunkRow][chunkCell].IsSaturated = dataOut[cell].IsSaturated;
    chunks[chunkCol][chunkRow][chunkCell].IsDetected = dataOut[cell].IsDetected;

    countByChunk[chunkCol][chunkRow]++;
  }

  H5Dclose(dataset);
  H5Fclose(file);
}

void HDF5CscanTest::WriteChunkedCscan(size_t gateOffset, std::vector<std::vector<std::vector<CompoundCscanData::OpenView>>>& chunks)
{
  auto file = H5Fopen("..\\CreateChunkedCscans.h5", H5F_ACC_RDWR, H5P_DEFAULT);

  auto dataset = H5Dopen(file, "CscanData", H5P_DEFAULT);
  auto filespace = H5Dget_space(dataset);
  auto dataTypeId = H5Dget_type(dataset);

  std::vector<hsize_t> cdims{ 57, 57 };
  std::vector<std::size_t> offset{ 0, 0, gateOffset };
  std::vector<std::size_t> count{ cdims[0], cdims[1], 1 };

  auto memspace = H5Screate_simple((int)cdims.size(), cdims.data(), nullptr);

  std::vector<std::vector<std::vector<CompoundCscanData::RDTIFF19::Rectified>>> newChunks;

  newChunks.resize(chunks.size());
  for (size_t col{}; col < chunks.size(); col++) {
    newChunks[col].resize(chunks[col].size());
    for (size_t row{}; row < chunks[col].size(); row++) {
      newChunks[col][row].resize(chunks[col][row].size());
      for (size_t cell{}; cell < chunks[row].size(); cell++)
      {
        newChunks[col][row][cell].Statuses = chunks[col][row][cell].HasData ? 1 : 0;
        newChunks[col][row][cell].Amplitude = chunks[col][row][cell].Amplitude;
        newChunks[col][row][cell].Time = static_cast<unsigned short>(chunks[col][row][cell].CrossingTime / 10.0);
      }
    }
  }

  size_t writeCounter{};
  auto timingBeginAll = std::chrono::high_resolution_clock::now();
  for (size_t col{}; col < chunks.size(); col++) {
    for (size_t row{}; row < chunks[col].size(); row++)
    {
      bool hasData{};
      for (size_t checkCell{}; checkCell < cdims[0] * cdims[1]; checkCell++) {
        hasData |= chunks[col][row][checkCell].HasData;
      }

      //if (hasData)
      {
        offset[0] = col * cdims[0];
        offset[1] = row * cdims[1];

        if (offset[1] < 862 - 57)
        {
          auto errorCode = H5Sselect_hyperslab(filespace, H5S_SELECT_SET, offset.data(), nullptr, count.data(), nullptr);
          if (errorCode != 0) {
            errorCode = 0;
          }

          errorCode = H5Dwrite(dataset, dataTypeId, memspace, filespace, H5P_DEFAULT, newChunks[col][row].data());
          if (errorCode != 0) {
            errorCode = 0;
          }
          else {
            writeCounter++;
          }
        }
      }
    }
  }

  H5Dclose(dataset);
  H5Fclose(file);

  std::cout << "Write chunk count " << writeCounter << std::endl;
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timingBeginAll);
  std::cout << duration.count() << " ms" << std::endl;
}

void HDF5CscanTest::ReadChunkedCscan()
{
  auto file = H5Fopen("..\\CreateChunkedCscans.h5", H5F_ACC_RDONLY, H5P_DEFAULT);
  auto dataset = H5Dopen(file, "CscanData", H5P_DEFAULT);
  auto filespace = H5Dget_space(dataset);

  auto rank = H5Sget_simple_extent_ndims(filespace);
  std::vector<hsize_t> dims(rank);
  H5Sget_simple_extent_dims(filespace, dims.data(), nullptr);

  std::vector<hsize_t> memdims{ dims[0], dims[1], dims[2] };
  auto memspace = H5Screate_simple((int)memdims.size(), memdims.data(), nullptr);
  std::vector<CompoundCscanData::RDTIFF19::Rectified> dataOut = std::vector<CompoundCscanData::RDTIFF19::Rectified>(memdims[0] * memdims[1] * memdims[2]);

  auto timingBegin = std::chrono::high_resolution_clock::now();
  auto errorCode = H5Dread(dataset, H5Dget_type(dataset), memspace, filespace, H5P_DEFAULT, dataOut.data());
  if (errorCode != 0) {
    errorCode = 0;
  }

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timingBegin);
  std::cout << "Read chunked " << duration.count() << " ms" << std::endl;
}
TEST_F(HDF5CscanTest, ReadWriteChunkCscans)
{
  CreateChunkedCscan();

  std::vector<std::vector<std::vector<CompoundCscanData::OpenView>>> chunksG1;
  ReadCscan(1, chunksG1);
  WriteChunkedCscan(1, chunksG1);

  std::vector<std::vector<std::vector<CompoundCscanData::OpenView>>> chunksG2;
  ReadCscan(2, chunksG2);
  WriteChunkedCscan(2, chunksG2);

  ReadChunkedCscan();


  // Write

  //{
  //  auto file = H5Fcreate("..\\CreateCscans.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  //  std::vector<hsize_t> memSize{ 14250, 862 };
  //  auto memspace = H5Screate_simple((int)memSize.size(), memSize.data(), nullptr);

  //  //const auto dataTypeId = H5Tcreate(H5T_COMPOUND, sizeof(CompoundCscanDataRDTIFF19));    
  //  //H5Tinsert(dataTypeId, "Statuses", HOFFSET(CompoundCscanDataRDTIFF19, Statuses), H5T_NATIVE_B8);
  //  //H5Tinsert(dataTypeId, "Amplitude", HOFFSET(CompoundCscanDataRDTIFF19, Amplitude), H5T_NATIVE_USHORT);
  //  //H5Tinsert(dataTypeId, "Time", HOFFSET(CompoundCscanDataRDTIFF19, Time), H5T_NATIVE_USHORT);

  //  const auto dataTypeId = H5Tcreate(H5T_COMPOUND, sizeof(CompoundCscanData));
  //  H5Tinsert(dataTypeId, "HasData", HOFFSET(CompoundCscanData, HasData), H5T_NATIVE_HBOOL);
  //  H5Tinsert(dataTypeId, "CrossingTime", HOFFSET(CompoundCscanData, CrossingTime), H5T_NATIVE_FLOAT);
  //  H5Tinsert(dataTypeId, "AmplitudeTime", HOFFSET(CompoundCscanData, PeakTime), H5T_NATIVE_FLOAT);
  //  H5Tinsert(dataTypeId, "Amplitude", HOFFSET(CompoundCscanData, Amplitude), H5T_NATIVE_INT);
  //  H5Tinsert(dataTypeId, "Synced", HOFFSET(CompoundCscanData, IsSyncedOnSource), H5T_NATIVE_HBOOL);
  //  H5Tinsert(dataTypeId, "Saturated", HOFFSET(CompoundCscanData, IsSaturated), H5T_NATIVE_HBOOL);
  //  H5Tinsert(dataTypeId, "Detected", HOFFSET(CompoundCscanData, IsDetected), H5T_NATIVE_HBOOL);
  //  
  //  auto dataset = H5Dcreate(file, "CscanData", dataTypeId, memspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  //  auto filespace = H5Dget_space(dataset);

  //  auto timingBegin = std::chrono::high_resolution_clock::now();
  //  auto erroCode = H5Dwrite(dataset, dataTypeId, H5S_ALL, H5S_ALL, H5P_DEFAULT, dataOut.data());
  //  if (errorCode != 0) {
  //    errorCode = 0;
  //  }

  //  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timingBegin);
  //  std::cout << "Write contiguous " << duration.count() << " ms" << std::endl;
  //}

  //// Read

  //{
  //  auto file = H5Fopen("..\\CreateCscans.h5", H5F_ACC_RDONLY, H5P_DEFAULT);
  //  auto dataset = H5Dopen(file, "CscanData", H5P_DEFAULT);
  //  auto filespace = H5Dget_space(dataset);

  //  auto rank = H5Sget_simple_extent_ndims(filespace);
  //  std::vector<hsize_t> dims(rank);
  //  H5Sget_simple_extent_dims(filespace, dims.data(), nullptr);

  //  std::vector<hsize_t> memdims{ dims[0], dims[1] };
  //  auto memspace = H5Screate_simple((int)memdims.size(), memdims.data(), nullptr);
  //  std::vector<CompoundCscanData> dataOut = std::vector<CompoundCscanData>(memdims[0] * memdims[1]);

  //  auto timingBegin = std::chrono::high_resolution_clock::now();
  //  errorCode = H5Dread(dataset, H5Dget_type(dataset), memspace, filespace, H5P_DEFAULT, dataOut.data());
  //  if (errorCode != 0) {
  //    errorCode = 0;
  //  }

  //  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timingBegin);
  //  std::cout << "Read " << duration.count() << " ms" << std::endl;
  //}
}
TEST_F(HDF5CscanTest, CreateLargeChunkedCscans)
{
  std::vector<CompoundCscanData::OpenView> dataOut;

  // Read original data

  {
    auto file = H5Fopen("..\\DataFile_3.h5", H5F_ACC_RDONLY, H5P_DEFAULT);
    auto dataset = H5Dopen(file, "/Domain/Data/Configurations/0/CscanData", H5P_DEFAULT);
    auto fileSpace = H5Dget_space(dataset);

    int rank = H5Sget_simple_extent_ndims(fileSpace);
    std::vector<hsize_t> dimensions(rank);
    H5Sget_simple_extent_dims(fileSpace, dimensions.data(), nullptr);

    std::vector<hsize_t> memSize{ dimensions[0], dimensions[1], 1 };
    auto memspace = H5Screate_simple((int)memSize.size(), memSize.data(), nullptr);

    std::cout << "Allocating: " << sizeof(CompoundCscanData::OpenView) * memSize[0] * memSize[1] * memSize[2] / 1073741824.0 << " GBytes" << std::endl;
    dataOut = std::vector<CompoundCscanData::OpenView>(memSize[0] * memSize[1] * memSize[2]);

    std::vector<std::size_t> offset{ 0 , 0 , 0 };
    std::vector<std::size_t> count{ memSize[0] , memSize[1] , memSize[2] };
    
    std::cout << "Reading cscans..." << std::endl;
    auto timingBeginAll = std::chrono::high_resolution_clock::now();
    for (size_t gate{1}; gate <= 1/*dimensions[2]*/; gate++)
    {
      offset[2] = gate;
      auto errorCode = H5Sselect_hyperslab(fileSpace, H5S_SELECT_SET, offset.data(), nullptr, count.data(), nullptr);

      std::cout << "Reading gate " << gate << std::endl;
      auto timingBegin = std::chrono::high_resolution_clock::now();
      errorCode = H5Dread(dataset, H5Dget_type(dataset), memspace, fileSpace, H5P_DEFAULT, dataOut.data());
      auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timingBegin);
      std::cout << duration.count() << " ms" << std::endl;
    }
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timingBeginAll);
    std::cout << duration.count() << " ms" << std::endl;

    H5Sclose(fileSpace);
    H5Dclose(dataset);
    H5Fclose(file);
  }

  // Write back chunked

  {
    auto file = H5Fcreate("..\\CreateLargeChunkedCscans.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    std::vector<hsize_t> memSize{ 14250, 862 };
    auto memspace = H5Screate_simple((int)memSize.size(), memSize.data(), nullptr);

    std::vector<hsize_t> cdims{ 57, 57 };
    auto plist = H5Pcreate(H5P_DATASET_CREATE);
    auto status = H5Pset_chunk(plist, (int)cdims.size(), cdims.data());

    const auto dataTypeId1 = H5Tcreate(H5T_COMPOUND, sizeof(CompoundCscanData::RDTIFF19::Rectified));
    //H5Tinsert(dataTypeId1, "Statuses", HOFFSET(CompoundCscanData::RDTIFF19::Rectified, Statuses), H5T_NATIVE_B8);
    //H5Tinsert(dataTypeId1, "Amplitude", HOFFSET(CompoundCscanData::RDTIFF19::Rectified, Amplitude), H5T_NATIVE_USHORT);
    //H5Tinsert(dataTypeId1, "Time", HOFFSET(CompoundCscanData::RDTIFF19::Rectified, Time), H5T_NATIVE_USHORT);

    //const auto dataTypeId2 = H5Tcreate(H5T_COMPOUND, sizeof(CompoundCscanData::OpenView));
    //H5Tinsert(dataTypeId2, "HasData", HOFFSET(CompoundCscanData::OpenView, HasData), H5T_NATIVE_HBOOL);
    //H5Tinsert(dataTypeId2, "CrossingTime", HOFFSET(CompoundCscanData::OpenView, CrossingTime), H5T_NATIVE_FLOAT);
    //H5Tinsert(dataTypeId2, "AmplitudeTime", HOFFSET(CompoundCscanData::OpenView, PeakTime), H5T_NATIVE_FLOAT);
    //H5Tinsert(dataTypeId2, "Amplitude", HOFFSET(CompoundCscanData::OpenView, Amplitude), H5T_NATIVE_INT);
    //H5Tinsert(dataTypeId2, "Synced", HOFFSET(CompoundCscanData::OpenView, IsSyncedOnSource), H5T_NATIVE_HBOOL);
    //H5Tinsert(dataTypeId2, "Saturated", HOFFSET(CompoundCscanData::OpenView, IsSaturated), H5T_NATIVE_HBOOL);
    //H5Tinsert(dataTypeId2, "Detected", HOFFSET(CompoundCscanData::OpenView, IsDetected), H5T_NATIVE_HBOOL);

    auto dataset = H5Dcreate(file, "CscanData", dataTypeId1, memspace, H5P_DEFAULT, plist, H5P_DEFAULT);

    auto fileSpace = H5Dget_space(dataset);

    std::vector<std::size_t> offset{ 0 , 0 };
    std::vector<std::size_t> count{ cdims[0] , cdims[1] };

    size_t writeCounter{};
    for (size_t chunkScan{}; chunkScan < 14250; chunkScan += 57)
    {
      if (chunkScan + 57 < 14250)
      {
        for (size_t chunkIndex{}; chunkIndex < 862; chunkIndex += 57)
        {
          if (chunkIndex + 57 < 862)
          {
            offset[0] = chunkScan;
            offset[1] = chunkIndex;

            size_t memOffset = chunkIndex + chunkScan * 862;

            auto errorCode = H5Sselect_hyperslab(fileSpace, H5S_SELECT_SET, offset.data(), nullptr, count.data(), nullptr);
            if (errorCode != 0) {
              errorCode = 0;
            }

            status = H5Dwrite(dataset, dataTypeId1, H5S_ALL, H5S_ALL, H5P_DEFAULT, dataOut.data());
            if (errorCode != 0) {
              errorCode = 0;
            }

            auto ptr = dataOut.data() + memOffset;
            if (ptr->HasData) {
              writeCounter++;
            }

          }
        }
      }
    }

    H5Sclose(memspace);
    H5Dclose(dataset);
    H5Fclose(file);
  }

  // Read chunked data

  {
    auto file = H5Fopen("..\\CreateLargeChunkedCscans.h5", H5F_ACC_RDONLY, H5P_DEFAULT);
    auto dataset = H5Dopen(file, "CscanData", H5P_DEFAULT);
    auto fileSpace = H5Dget_space(dataset);

    int rank = H5Sget_simple_extent_ndims(fileSpace);
    std::vector<hsize_t> dimensions(rank);
    H5Sget_simple_extent_dims(fileSpace, dimensions.data(), nullptr);

    std::vector<hsize_t> memSize{ 57, 57 };
    auto memspace = H5Screate_simple((int)memSize.size(), memSize.data(), nullptr);

    dataOut = std::vector<CompoundCscanData::OpenView>(dimensions[0] * dimensions[1] );

    std::vector<std::size_t> offset{ 0 , 0 };
    std::vector<std::size_t> count{ memSize[0] , memSize[1] };

    std::cout << "Reading chunks " << std::endl;
    auto timingBegin = std::chrono::high_resolution_clock::now();

    size_t readCounter{};
    for (size_t chunkScan{}; chunkScan < dimensions[0]; chunkScan +=57)
    {
      if (chunkScan + 57 < dimensions[0])
      {
        for (size_t chunkIndex{}; chunkIndex < dimensions[1]; chunkIndex += 57)
        {
          if (chunkIndex + 57 < dimensions[1])
          {
            offset[0] = chunkScan;
            offset[1] = chunkIndex;

            size_t memOffset = chunkIndex + chunkScan * dimensions[1];

            auto errorCode = H5Sselect_hyperslab(fileSpace, H5S_SELECT_SET, offset.data(), nullptr, count.data(), nullptr);
            errorCode = H5Dread(dataset, H5Dget_type(dataset), memspace, fileSpace, H5P_DEFAULT, dataOut.data() + memOffset);
            if (errorCode != 0) {
              errorCode = 0;
            }

            auto ptr = dataOut.data() + memOffset;
            if (ptr->HasData) {
              readCounter++;
            }
            
          }
        }
      }
    }

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timingBegin);
    std::cout << duration.count() << " ms" << std::endl;
    std::cout << "Count " << readCounter << std::endl;
 
    H5Sclose(fileSpace);
    H5Dclose(dataset);
    H5Fclose(file);
  }
}
TEST_F(HDF5CscanTest, ReadScans)
{
  auto file = H5Fopen("..\\DataFile_3.h5", H5F_ACC_RDONLY, H5P_DEFAULT);
  auto dataset = H5Dopen(file, "/Domain/Data/Configurations/0/AscanData", H5P_DEFAULT);

  auto dataspace = H5Dget_space(dataset);

  // Define the memory dataspace.
  hsize_t dimsm[2];
  dimsm[0] = 862;
  dimsm[1] = 1220;

  auto memspace = H5Screate_simple(2, dimsm, NULL);

  auto timingBegin = std::chrono::high_resolution_clock::now();

  uint64_t scans(14250);
  size_t dataSize(scans * 862 * 1220);
  std::cout << "Allocating: " << dataSize / 1073741824.0 << " GBytes" << std::endl;
  std::vector<uint8_t> dataOut(dataSize, 0);
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timingBegin);
  std::cout << duration.count() << " ms" << std::endl;

  hsize_t offset[3]{};
  hsize_t count[3];
  count[0] = 1;
  count[1] = 862;
  count[2] = 1220;

  herr_t status{};
  
  std::cout << "Reading: " << scans << " scan" << std::endl;
  timingBegin = std::chrono::high_resolution_clock::now();
  for (uint64_t i{}; i < scans; i++)
  {
    // Define hyperslab in the dataset.
    offset[0] = i;
    status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset, NULL, count, NULL);

    status = H5Dread(dataset, H5Dget_type(dataset), memspace, dataspace, H5P_DEFAULT, &dataOut[i * 862 * 1220]);
    if (status != 0) {
      break;
    }
  }

  if (status == 0)
  {
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timingBegin);
    std::cout << duration.count() << " ms" << std::endl;
  }

  H5Fclose(file);
}

TEST_F(HDF5CscanTest, ReadHalfScans)
{
  auto file = H5Fopen("..\\DataFile_3.h5", H5F_ACC_RDONLY, H5P_DEFAULT);
  auto dataset = H5Dopen(file, "/Domain/Data/Configurations/0/AscanData", H5P_DEFAULT);

  auto dataspace = H5Dget_space(dataset);

  // Define the memory dataspace.
  hsize_t dimsm[2];
  dimsm[0] = 431;
  dimsm[1] = 1220;

  auto memspace = H5Screate_simple(2, dimsm, NULL);

  auto timingBegin = std::chrono::high_resolution_clock::now();

  uint64_t scans(14250);
  size_t dataSize(scans * 862 * 1220);
  std::cout << "Allocating: " << dataSize / 1073741824.0 << " GBytes" << std::endl;
  std::vector<uint8_t> dataOut(dataSize, 0);
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timingBegin);
  std::cout << duration.count() << " ms" << std::endl;

  hsize_t offset[3]{0, 215, 0};
  hsize_t count[3];
  count[0] = 1;
  count[1] = 431;
  count[2] = 1220;

  herr_t status{};

  std::cout << "Reading: " << scans << " scan" << std::endl;
  timingBegin = std::chrono::high_resolution_clock::now();
  for (uint64_t i{}; i < scans; i++)
  {
    // Define hyperslab in the dataset.
    offset[0] = i;
    status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset, NULL, count, NULL);

    status = H5Dread(dataset, H5Dget_type(dataset), memspace, dataspace, H5P_DEFAULT, &dataOut[i * 862 * 1220]);
    if (status != 0) {
      break;
    }
  }

  if (status == 0)
  {
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timingBegin);
    std::cout << duration.count() << " ms" << std::endl;
  }

  H5Fclose(file);
}
TEST_F(HDF5CscanTest, Foo2)
{
  hid_t fileId = H5Fcreate("data.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  hsize_t dims1[3]{ 2, 3, 4 };
  hid_t filespaceId = H5Screate_simple(3, dims1, nullptr);
  hid_t datasetId = H5Dcreate(fileId, "Data", H5T_NATIVE_USHORT, filespaceId, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

  hsize_t count[]{ 1, 3, 4 };
  hid_t memspaceId = H5Screate_simple(3, count, NULL);
  size_t offset[]{ 0, 0, 0 };

  std::vector<unsigned short> ascan(2 * 3 * 4);

  size_t index{};
  unsigned short height{2};
  unsigned short width{3};
  unsigned short depth{4};
  unsigned short sample{};
  for (size_t dim0{}; dim0 < height; dim0++) { //height - row - index (skew 90/270 contexts)
    for (size_t dim1{}; dim1 < width; dim1++) { //width - col - scan (skew 0/180 contexts)
      for (size_t dim2{}; dim2 < depth; dim2++) { //depth - samples - ultrasound
        ascan[index++] = ++sample;
      }
      sample = (sample - depth) + 10;
    }
    sample = (sample - width * 10) + 100;
  }


  for (size_t dim0{}; dim0 < height; dim0++) //height - row - index (skew 90/270 contexts)
  {
    offset[0] = dim0;
    herr_t status = H5Sselect_hyperslab(filespaceId, H5S_SELECT_SET, offset, NULL, count, NULL);
    status = H5Dwrite(datasetId, H5T_NATIVE_USHORT, memspaceId, filespaceId, H5P_DEFAULT, ascan.data() + dim0 * width * depth);
    status = 0;
  }

  H5Fclose(fileId);
}
