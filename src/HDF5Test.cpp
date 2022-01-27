#include "HDF5Test.h"

#include <unordered_map>

#include <hdf5/hdf5.h>
#include <hdf5/hdf5_hl.h>


namespace Axis
{
	constexpr char Scan[] = "Scan";
	constexpr char Index[] = "Index";
};

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

TEST_F(HDF5Test, Test1)
{

	std::unordered_map<std::string, size_t> dimensions = { {Axis::Scan, 1}, {Axis::Index, 2} };

	auto s = dimensions[Axis::Index];

	dimensions.clear();
}

#define RANK          2
#define DIM_DATA      12
#define DIM1_SIZE     3
#define DIM2_SIZE     4
#define DIM0          0
#define DIM1          1

#define DSET_NAME      "Mydata"
#define DS_1_NAME      "Yaxis"
#define DS_2_NAME      "Xaxis"

TEST_F(HDF5Test, Test2)
{
  hid_t   fid;                                              /* file ID */
  hid_t   did;                                              /* dataset ID */
  hid_t   dsid;                                             /* DS dataset ID */
  int     rank = RANK;                                  /* rank of data dataset */
  int     rankds = 1;                                     /* rank of DS dataset */
  hsize_t dims[RANK] = { DIM1_SIZE,DIM2_SIZE };              /* size of data dataset */
  int     buf[DIM_DATA] = { 1,2,3,4,5,6,7,8,9,10,11,12 };     /* data of data dataset */
  hsize_t s1_dim[1] = { DIM1_SIZE };                         /* size of DS 1 dataset */
  hsize_t s2_dim[1] = { DIM2_SIZE };                         /* size of DS 2 dataset */
  float   s1_wbuf[DIM1_SIZE] = { 10,20,30 };                  /* data of DS 1 dataset */
  int     s2_wbuf[DIM2_SIZE] = { 10,20,50,100 };              /* data of DS 2 dataset */


  /* create a file using default properties */
  if ((fid = H5Fcreate("ex_ds1.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT)) < 0)
    goto out;

  /* make a dataset */
  if (H5LTmake_dataset_int(fid, DSET_NAME, rank, dims, buf) < 0)
    goto out;

  /* make a DS dataset for the first dimension */
  if (H5LTmake_dataset_float(fid, DS_1_NAME, rankds, s1_dim, s1_wbuf) < 0)
    goto out;

  /* make a DS dataset for the second dimension */
  if (H5LTmake_dataset_int(fid, DS_2_NAME, rankds, s2_dim, s2_wbuf) < 0)
    goto out;


  /*-------------------------------------------------------------------------
   * attach the DS_1_NAME dimension scale to DSET_NAME at dimension 0
   *-------------------------------------------------------------------------
   */

   /* get the dataset id for DSET_NAME */
  if ((did = H5Dopen2(fid, DSET_NAME, H5P_DEFAULT)) < 0)
    goto out;

  /* get the DS dataset id */
  if ((dsid = H5Dopen2(fid, DS_1_NAME, H5P_DEFAULT)) < 0)
    goto out;

  /* attach the DS_1_NAME dimension scale to DSET_NAME at dimension index 0 */
  if (H5DSattach_scale(did, dsid, DIM0) < 0)
    goto out;

  /* close DS id */
  if (H5Dclose(dsid) < 0)
    goto out;

  /*-------------------------------------------------------------------------
   * attach the DS_2_NAME dimension scale to DSET_NAME
   *-------------------------------------------------------------------------
   */

   /* get the DS dataset id */
  if ((dsid = H5Dopen2(fid, DS_2_NAME, H5P_DEFAULT)) < 0)
    goto out;

  /* attach the DS_2_NAME dimension scale to DSET_NAME as the 2nd dimension (index 1)  */
  if (H5DSattach_scale(did, dsid, DIM1) < 0)
    goto out;

  /* close DS id */
  if (H5Dclose(dsid) < 0)
    goto out;

  /* close file */
  H5Fclose(fid);

  return;

out:
  printf("Error on return function...Exiting\n");
  return;
}

//

#define H5FILE_NAME "Attributes.h5"

//#define RANK  1   /* Rank and size of the dataset  */
//#define SIZE  7
//
//#define ARANK  2   /* Rank and dimension sizes of the first dataset attribute */
//#define ADIM1  2
//#define ADIM2  3
//#define ANAME  "Float attribute"      /* Name of the array attribute */
//#define ANAMES "Character attribute" /* Name of the string attribute */

/*
 * Operator function.
 */
static herr_t
attr_info(hid_t loc_id, const char* name, const H5A_info_t* ainfo, void* opdata)
{
  //hid_t attr, atype, aspace;  /* Attribute, datatype and dataspace identifiers */
  //int   rank;
  //hsize_t sdim[64];
  //herr_t ret;
  //int i;
  //size_t npoints;             /* Number of elements in the array attribute. */
  //float* float_array;         /* Pointer to the array attribute. */

  ///* avoid warnings */
  //opdata = opdata;

  ///*
  // * Open the attribute using its name.
  // */
  //attr = H5Aopen(loc_id, name, H5P_DEFAULT);

  ///*
  // * Display attribute name.
  // */
  //printf("\nName : %s\n", name);

  ///*
  // * Get attribute datatype, dataspace, rank, and dimensions.
  // */
  //atype = H5Aget_type(attr);
  //aspace = H5Aget_space(attr);
  //rank = H5Sget_simple_extent_ndims(aspace);
  //ret = H5Sget_simple_extent_dims(aspace, sdim, NULL);

  ///*
  // *  Display rank and dimension sizes for the array attribute.
  // */

  //if (rank > 0) {
  //  printf("Rank : %d \n", rank);
  //  printf("Dimension sizes : ");
  //  for (i = 0; i < rank; i++)
  //    printf("%d ", (int)sdim[i]);
  //  printf("\n");
  //}

  ///*
  // * Read array attribute and display its type and values.
  // */

  //if (H5T_FLOAT == H5Tget_class(atype)) {
  //  printf("Type : FLOAT \n");
  //  npoints = H5Sget_simple_extent_npoints(aspace);
  //  float_array = (float*)malloc(sizeof(float) * (int)npoints);
  //  ret = H5Aread(attr, atype, float_array);
  //  printf("Values : ");
  //  for (i = 0; i < (int)npoints; i++)
  //    printf("%f ", float_array[i]);
  //  printf("\n");
  //  free(float_array);
  //}

  ///*
  // * Release all identifiers.
  // */
  //H5Tclose(atype);
  //H5Sclose(aspace);
  //H5Aclose(attr);

  return 0;
}

//TEST_F(HDF5Test, Test3)
//{
//  hid_t   file, dataset;       /* File and dataset identifiers */
//
//  hid_t   fid;                 /* Dataspace identifier */
//  hid_t   attr1, attr2, attr3; /* Attribute identifiers */
//  hid_t   attr;
//  hid_t   aid1, aid2, aid3;    /* Attribute dataspace identifiers */
//  hid_t   atype, atype_mem;    /* Attribute type */
//  H5T_class_t  type_class;
//
//  hsize_t fdim[] = { SIZE };
//  hsize_t adim[] = { ADIM1, ADIM2 };  /* Dimensions of the first attribute  */
//
//  float matrix[ADIM1][ADIM2]; /* Attribute data */
//
//  herr_t  ret;                /* Return value */
//  H5O_info2_t oinfo;          /* Object info */
//  unsigned i, j;              /* Counters */
//  char    string_out[80];     /* Buffer to read string attribute back */
//  int     point_out;          /* Buffer to read scalar attribute back */
//
//  /*
//   * Data initialization.
//   */
//  int vector[] = { 1, 2, 3, 4, 5, 6, 7 };  /* Dataset data */
//  int point = 1;                         /* Value of the scalar attribute */
//  char string[] = "ABCD";                /* Value of the string attribute */
//
//
//  for (i = 0; i < ADIM1; i++) {            /* Values of the array attribute */
//    for (j = 0; j < ADIM2; j++)
//      matrix[i][j] = -1.;
//  }
//
//  /*
//   * Create a file.
//   */
//  file = H5Fcreate(H5FILE_NAME, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
//
//  /*
//   * Create the dataspace for the dataset in the file.
//   */
//  fid = H5Screate(H5S_SIMPLE);
//  ret = H5Sset_extent_simple(fid, RANK, fdim, NULL);
//
//  /*
//   * Create the dataset in the file.
//   */
//  dataset = H5Dcreate2(file, "G1/Dataset", H5T_NATIVE_INT, fid, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
//
//  /*
//   * Write data to the dataset.
//   */
//  ret = H5Dwrite(dataset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, vector);
//
//  /*
//   * Create dataspace for the first attribute.
//   */
//  aid1 = H5Screate(H5S_SIMPLE);
//  ret = H5Sset_extent_simple(aid1, ARANK, adim, NULL);
//
//  /*
//   * Create array attribute.
//   */
//  attr1 = H5Acreate2(dataset, ANAME, H5T_NATIVE_FLOAT, aid1, H5P_DEFAULT, H5P_DEFAULT);
//
//  /*
//   * Write array attribute.
//   */
//  ret = H5Awrite(attr1, H5T_NATIVE_FLOAT, matrix);
//
//
//
//  //
//
//  hid_t datasetX = H5Dcreate2(file, "DatasetX", H5T_NATIVE_INT, fid, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
//
//  hsize_t adim2[] = { 3 };
//
//  hid_t aid4 = H5Screate(H5S_SIMPLE);
//  ret = H5Sset_extent_simple(aid4, 1, adim2, NULL);
//
//  //H5T_VARIABLE
//  hid_t atype2 = H5Tcopy(H5T_C_S1);
//  H5Tset_size(atype2, H5T_VARIABLE);
//  H5Tset_strpad(atype2, H5T_STR_NULLTERM);
//  H5Tset_cset(atype2, H5T_CSET_UTF8);
//
//  attr1 = H5Acreate2(datasetX, "Labels", atype2, aid4, H5P_DEFAULT, H5P_DEFAULT);
//
//
//  std::vector<std::string> att_vector;
//  att_vector.push_back("ColName1");
//  att_vector.push_back("ColName2 more characters");
//  att_vector.push_back("ColName3");
//
//  std::vector<const char*> cStrArray;
//  for (int index = 0; index < att_vector.size(); ++index) {
//    cStrArray.push_back(att_vector[index].c_str());
//  }
//
//
//  ret = H5Awrite(attr1, atype2, (void*)&cStrArray[0]);
//
//
//
//
//  //std::vector<size_t> attributeSize = { dimensionAxes_.size() };
//  //dataspaceId = H5Screate_simple((int)attributeSize.size(), attributeSize.data(), nullptr);
//  //hid_t attributeType = H5Tcopy(H5T_C_S1);
//  //H5Tset_size(attributeType, H5T_VARIABLE);
//  //H5Tset_strpad(attributeType, H5T_STR_NULLTERM);
//  //H5Tset_cset(attributeType, H5T_CSET_UTF8);
//
//  //hid_t attributeId = H5Acreate2(m_datasetId, "DimensionLabels", attributeType, dataspaceId, H5P_DEFAULT, H5P_DEFAULT);
//  //herr_t errorCode = H5Awrite(attributeId, attributeType, (void*)&attributeLabels[0]);
//  //H5Sclose(dataspaceId);
//  //H5Aclose(attributeId);
//
//
//
//
//
//  /*
//   * Create scalar attribute.
//   */
//  aid2 = H5Screate(H5S_SCALAR);
//  attr2 = H5Acreate2(dataset, "Integer attribute", H5T_NATIVE_INT, aid2,
//    H5P_DEFAULT, H5P_DEFAULT);
//
//  /*
//   * Write scalar attribute.
//   */
//  ret = H5Awrite(attr2, H5T_NATIVE_INT, &point);
//
//  /*
//   * Create string attribute.
//   */
//  aid3 = H5Screate(H5S_SCALAR);
//  atype = H5Tcopy(H5T_C_S1);
//  H5Tset_size(atype, 5);
//  H5Tset_strpad(atype, H5T_STR_NULLTERM);
//  attr3 = H5Acreate2(dataset, ANAMES, atype, aid3, H5P_DEFAULT, H5P_DEFAULT);
//
//  /*
//   * Write string attribute.
//   */
//  ret = H5Awrite(attr3, atype, string);
//
//  /*
//   * Close attribute and file dataspaces, and datatype.
//   */
//  ret = H5Sclose(aid1);
//  ret = H5Sclose(aid2);
//  ret = H5Sclose(aid3);
//  ret = H5Sclose(fid);
//  ret = H5Tclose(atype);
//
//  /*
//   * Close the attributes.
//   */
//  ret = H5Aclose(attr1);
//  ret = H5Aclose(attr2);
//  ret = H5Aclose(attr3);
//
//  /*
//   * Close the dataset.
//   */
//  ret = H5Dclose(dataset);
//
//  /*
//   * Close the file.
//   */
//  ret = H5Fclose(file);
//
//  /*
//   * Reopen the file.
//   */
//  file = H5Fopen(H5FILE_NAME, H5F_ACC_RDONLY, H5P_DEFAULT);
//
//  /*
//   * Open the dataset.
//   */
//  dataset = H5Dopen2(file, "Dataset", H5P_DEFAULT);
//
//  /*
//   * Attach to the scalar attribute using attribute name, then read and
//   * display its value.
//   */
//  attr = H5Aopen(dataset, "Integer attribute", H5P_DEFAULT);
//  ret = H5Aread(attr, H5T_NATIVE_INT, &point_out);
//  printf("The value of the attribute \"Integer attribute\" is %d \n", point_out);
//  ret = H5Aclose(attr);
//
//  /*
//   * Find string attribute by iterating through all attributes
//   */
//  ret = H5Oget_info3(dataset, &oinfo, H5O_INFO_NUM_ATTRS);
//  for (i = 0; i < (unsigned)oinfo.num_attrs; i++) {
//    attr = H5Aopen_by_idx(dataset, ".", H5_INDEX_CRT_ORDER, H5_ITER_INC, (hsize_t)i, H5P_DEFAULT, H5P_DEFAULT);
//    atype = H5Aget_type(attr);
//    type_class = H5Tget_class(atype);
//    if (type_class == H5T_STRING) {
//      atype_mem = H5Tget_native_type(atype, H5T_DIR_ASCEND);
//      ret = H5Aread(attr, atype_mem, string_out);
//      printf("Found string attribute; its index is %d , value =   %s \n", i, string_out);
//      ret = H5Tclose(atype_mem);
//    }
//    ret = H5Aclose(attr);
//    ret = H5Tclose(atype);
//  }
//
//  /*
//   * Get attribute info using iteration function.
//   */
//  ret = H5Aiterate2(dataset, H5_INDEX_NAME, H5_ITER_INC, NULL, attr_info, NULL);
//
//  /*
//   * Close the dataset and the file.
//   */
//  H5Dclose(dataset);
//  H5Fclose(file);
//}

TEST_F(HDF5Test, Test4)
{

}