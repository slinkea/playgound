#pragma once
#pragma warning(push, 0)
#include <hdf5/H5Cpp.h>
#pragma warning(pop)
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

constexpr char FLE_VERSION[] = "File Version";
constexpr size_t MAX_FILEVERSION_LENGTH = 256;

//TODO[SVC][Mettre ca dans ONDTLib]

class H5Utils
{
public:
  static std::wstring ReadAttributeWString(hid_t id_, const std::string& name_)
  {
    std::wstring out;
    hid_t attr = H5Aopen(id_, name_.c_str(), H5P_DEFAULT);
    hid_t dspaceId = H5Aget_space(attr);
    H5S_class_t dspaceClass = H5Sget_simple_extent_type(dspaceId);
    if (dspaceClass == H5S_SIMPLE)
    {
      hsize_t hdim = 0;
      H5Sget_simple_extent_dims(dspaceId, &hdim, NULL);

      std::vector<wchar_t> value(hdim);
      hid_t attrFtype = H5Aget_type(attr);
      herr_t status = H5Aread(attr, attrFtype, value.data());
      if (status == 0)
      {
        out = std::wstring(value.begin(), value.end());
      }
    }

    return out;
  }

  template <typename T>
  static T ReadAttributeT(hid_t id_, const std::string& name_)
  {
    hid_t attr = H5Aopen(id_, name_.c_str(), H5P_DEFAULT);
    hid_t attrFtype = H5Aget_type(attr);
    T value;
    herr_t status = H5Aread(attr, attrFtype, &value);

    return value;
  }

  static size_t Size(const fs::path& filePath_)
  {
    hsize_t fileSize{};
    hid_t fileId = H5Fopen(filePath_.string().c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    H5Fget_filesize(fileId, &fileSize);
    H5Fclose(fileId);

    return fileSize;
  }

  static std::string GetFileVersion(hid_t fileId_)
  {
    char fileVersion[MAX_FILEVERSION_LENGTH]{};
    hid_t attrId = H5Aopen(fileId_, FLE_VERSION, H5P_DEFAULT);
    hid_t attrType = H5Aget_type(attrId);
    H5Aread(attrId, attrType, fileVersion);
    H5Aclose(attrId);

    return std::string(fileVersion);
  }

  static void GetConfigurations(hid_t fileId_)
  {

  }
};
