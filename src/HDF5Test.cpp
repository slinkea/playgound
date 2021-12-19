#include "HDF5Test.h"

#include <hdf5/hdf5.h>

constexpr char PATH_FILENAME[] = "data.h5";


HDF5Test::HDF5Test()
{
}

HDF5Test::~HDF5Test()
{
}

void HDF5Test::SetUp()
{
}

void HDF5Test::TearDown()
{
}

TEST_F(HDF5Test, Foo)
{
  hid_t fileId = H5Fcreate(PATH_FILENAME, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

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
