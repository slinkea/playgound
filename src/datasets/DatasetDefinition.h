#pragma once


constexpr char GROUP_DATA[] = "/Data";
constexpr char ASCAN_DATASET[] = "Ascan Data";
constexpr char ASCAN_STATUS_DATASET[] = "Ascan Status";
constexpr char BEAM_PREFIX[] = "Beam";
constexpr size_t MAX_NAME_LENGTH = 512;

#define H5_RESULT_CHECK( ret )                \
    if ( ret < 0 ) {                          \
    throw std::runtime_error("HDF5 Error.");  \
    }

struct CscanData1
{
  CscanData1() = default;
  CscanData1(float position, int amplitude, bool isSyncedOnSource, bool isSaturated, bool isDetected)
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

struct CscanData2 : CscanData1
{
  CscanData2() = default;
  CscanData2(float position, float crossingTime, float amplitudeTime, int amplitude, bool isSyncedOnSource, bool isSaturated, bool isDetected)
    : CscanData1(position, amplitude, isSyncedOnSource, isSaturated, isDetected)
    , CrossingTime(crossingTime)
    , AmplitudeTime(amplitudeTime)
  {
  }

  float CrossingTime{};
  float AmplitudeTime{};
};

using CscanData = CscanData2;

struct DataAxis
{
  double Min{ 0.0 };
  double Max{ 0.0 };
  double Resolution{ 0.0 };
  std::string Unit;
};

struct CscanAttributes
{
  CscanAttributes() = default;
  
  int AmpSamplingMin{};
  int AmpSamplingMax{};
  unsigned short AmpSamplingResol{};
  double PositionMax{};
  double PositionResol{};
  DataAxis Scan;
  DataAxis Index;
};

struct AscanAttributes
{
  AscanAttributes() = default;

  int AmpSamplingRange[2]{};
  unsigned short AmpSamplingResol{};  
};

struct DataDimensions
{
  DataDimensions() = default;

  DataDimensions(size_t x_)
    : X(x_)
  {
  }

  DataDimensions(size_t x_, size_t y_)
    : X(x_)
    , Y(y_)
  {
  }

  DataDimensions(size_t x_, size_t y_, size_t z_)
    : X(x_)
    , Y(y_)
    , Z(z_)
  {
  }

  size_t X{};
  size_t Y{};
  size_t Z{};
};

struct DataProperties
{
  DataProperties() = default;
  DataProperties(bool isChunked_, const DataDimensions& dims_)
    : IsChunked(isChunked_)
    , Dimensions(dims_)
  {
  }

  bool IsChunked{};
  DataDimensions Dimensions;
};
