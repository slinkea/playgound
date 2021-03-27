#pragma once
#include <map>
#include <string>
#include <vector>
#include <utility>
#include <exception>


// Version 1.2.0
constexpr char GROUP_DATA[] = "/Data";
constexpr char ASCAN_DATASET[] = "Ascan Data";
constexpr char ASCAN_STATUS_DATASET[] = "Ascan Status";
constexpr char CSCAN_GROUP[] = "Cscan";
constexpr char BEAM_PREFIX[] = "Beam";
constexpr char AMP_SAMP_MIN[] = "AmplitudeSamplingMin";
constexpr char AMP_SAMP_MAX[] = "AmplitudeSamplingMax";

constexpr size_t MAX_NAME_LENGTH = 512;

#define H5_RESULT_CHECK( ret )                \
    if ( ret < 0 ) {                          \
    throw std::runtime_error("HDF5 Error.");  \
    }

enum class DataType { CHAR, UCHAR, SHORT, USHORT, INT, UINT, FLOAT, DOUBLE };

using TGateIdentifiers = std::map<size_t, std::wstring>;

struct CscanDataMember1
{
  CscanDataMember1() = default;
  CscanDataMember1(float position, int amplitude, bool isSyncedOnSource, bool isSaturated, bool isDetected)
    : Position(position)
    , Amplitude(amplitude)
    , IsSyncedOnSource(isSyncedOnSource)
    , IsSaturated(isSaturated)
    , IsDetected(isDetected)
    , HasData(true)
  {
  }

  float Position{};
  int Amplitude{};
  bool IsSyncedOnSource{};
  bool IsSaturated{};
  bool IsDetected{};
  bool HasData{};
};

struct CscanDataMember2 : CscanDataMember1
{
  CscanDataMember2() = default;
  CscanDataMember2(float position, float crossingTime, float amplitudeTime, int amplitude, bool isSyncedOnSource, bool isSaturated, bool isDetected)
    : CscanDataMember1(position, amplitude, isSyncedOnSource, isSaturated, isDetected)
    , CrossingTime(crossingTime)
    , AmplitudeTime(amplitudeTime)
  {
  }

  float CrossingTime{};
  float AmplitudeTime{};
};

using CscanDataMember = CscanDataMember2;

struct DataRangeFloat
{
  float Min{ 0.0 };
  float Max{ 0.0 };
  float Resolution{ 0.0 };
  std::string Unit;
};

struct DataRangeShort
{
  short Min{ 0 };
  short Max{ 0 };
  short Resolution{ 0 };
  std::string Unit;
};

struct CscanAttributes
{
  CscanAttributes() = default;
  
  DataRangeFloat Samples;
  DataRangeFloat X;
  DataRangeFloat Y;
};

struct AscanAttributes
{
  AscanAttributes() = default;

  DataRangeShort AmplitudeSampling;
  DataRangeFloat Ultrasound;
};

struct DataDimensions
{
  DataDimensions() = default;

  DataDimensions(size_t sizeX_)
    : SizeX(sizeX_)
    , Quantity(1)
  {
  }

  DataDimensions(size_t sizeX_, size_t sizeY_)
    : SizeX(sizeX_)
    , SizeY(sizeY_)
    , Quantity(2)
  {
  }

  DataDimensions(size_t sizeX_, size_t sizeY_, size_t sizeZ_)
    : SizeX(sizeX_)
    , SizeY(sizeY_)
    , SizeZ(sizeZ_)
    , Quantity(3)
  {
  }

  size_t SizeX{};
  size_t SizeY{};
  size_t SizeZ{};
  size_t Quantity{};
};
