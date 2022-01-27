#pragma once
#include <gtest/gtest.h>


struct CompoundCscanData
{
  struct RDTIFF19 // 5 bytes
  {
    struct Rectified // 72MB
    {
      int32_t Amplitude{};  // 48MB
      uint16_t Time{};      // 24MB
      uint8_t Statuses{};   // 12MB
    };

    struct Unrectified
    {
      uint8_t Statuses{};
      int16_t Amplitude{};
      uint16_t Time{};
    };
  };

  struct ExtensibleFile // 24 bytes
  {
    uint32_t StatusMask{};
    int32_t Peak{};
    float AbsoluteStart{};
    float AbsoluteEnd{};
    float AbsolutePositionPeak{};
    float AbsolutePositionCrossing{};
  };

  struct OpenView // 20 bytes
  {
    OpenView() = default;
    OpenView(float crossingTime, float peakTime,
      int amplitude, bool isSyncedOnSource, bool isSaturated, bool isDetected)
      : HasData(true)
      , CrossingTime(crossingTime)
      , PeakTime(peakTime)
      , Amplitude(amplitude)
      , IsSyncedOnSource(isSyncedOnSource)
      , IsSaturated(isSaturated)
      , IsDetected(isDetected)
    {
    }

    float CrossingTime{};
    float PeakTime{};
    int Amplitude{};
    bool HasData{};
    bool IsSyncedOnSource{};
    bool IsSaturated{};
    bool IsDetected{};
  };
};


class HDF5CscanTest : public testing::Test
{
public:
  HDF5CscanTest();
	virtual ~HDF5CscanTest();

protected:
	void SetUp() override;
	void TearDown() override;

	void ReadCscan(size_t gateOffset, std::vector<std::vector<std::vector<CompoundCscanData::OpenView>>>& chunks);
	void CreateChunkedCscan();
  void WriteChunkedCscan(size_t gateOffset, std::vector<std::vector<std::vector<CompoundCscanData::OpenView>>>& chunks);
  void ReadChunkedCscan();

};
