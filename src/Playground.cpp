#include <iostream>
#include <zmq/zmq.h>
#include <cnpy/cnpy.h>
#include <log4cplus/log4cplus.h>
#include <rapidjson/document.h>
#include <boost/asio.hpp>
#include <boost/process.hpp>
#include <opencv2/opencv.hpp>

#pragma warning(push)
#pragma warning(disable: 4138)
#pragma warning(disable: 4267)
#include <uwebsockets/App.h>
#pragma warning(pop)

#include "Playground.h"
#include "Storage/NdtData.hpp"
#include "FileStorage.hpp"
#include "CudaKernels.cuh"


//constexpr char FILENAME[] = "big.h5";
constexpr char FILENAME_1[] = "D:\\NDT Files\\ThinBlade.h5";
constexpr char FILENAME_2[] = "D:\\NDT Files\\4-Configs.h5";


struct us_listen_socket_t* global_listen_socket;

void ReadAscanData(const AscanDataset* dset_)
{
  const AscanAttributes& attributes = dset_->Attributes();
  const DatasetProperties& props = dset_->Properties();
  const DataDimensions& dims = dset_->Dimensions();
  DataType sampleType = dset_->SampleType();
  size_t sampleSize = dset_->SampleSize();

  dset_->SelectSingle(1, 0);
  std::vector<unsigned char> singleAscan(dims.SizeZ * sampleSize, 0);
  dset_->Read(singleAscan.data());

  std::vector<short> ascanSamples;
  if (sampleType == DataType::CHAR) {
    const char* samples = reinterpret_cast<const char*>(singleAscan.data());
    ascanSamples.assign(samples, samples + dims.SizeZ);
  }
  else if (sampleType == DataType::SHORT) {
    const short* samples = reinterpret_cast<const short*>(singleAscan.data());
    ascanSamples.assign(samples, samples + dims.SizeZ);
  }
  else if (sampleType == DataType::UCHAR) {
    const unsigned char* samples = reinterpret_cast<const unsigned char*>(singleAscan.data());
    ascanSamples.assign(samples, samples + dims.SizeZ);
  }
  else if (sampleType == DataType::USHORT) {
    const unsigned short* samples = reinterpret_cast<const unsigned short*>(singleAscan.data());
    ascanSamples.assign(samples, samples + dims.SizeZ);
  }

  if (const auto ascanBeamDset = dynamic_cast<const AscanBeamDataset*>(dset_))
  {
    size_t idx = ascanBeamDset->BeamIndex();
    idx = 0;
  }
}

void ReadCscanData(const CscanDataset* dset_)
{
}


int main(int argc, char* argv[])
{
  try
  {
    //FileStorage fs;
    //fs.ReadBig();
    //fs.CompressFile();

    NdtData ndtData;

    size_t fileSize1 = H5Utils::Size(FILENAME_1);
    size_t fileSize2 = H5Utils::Size(FILENAME_2);

    //acquiredData.Open(FILENAME_1);
    ndtData.Open(FILENAME_2);

    //const auto& dataContainer1 = acquiredData.GetDataContainer(FILENAME_1);
    //const auto& fileVersion = dataContainer1.Version();

    const auto& dataContainer2 = ndtData.GetDataContainer(FILENAME_2);

    for (const auto& dataItem : dataContainer2.Items())
    {
      const auto& src = dataItem->Source();
      const std::wstring& configName = src.ConfigName();
      size_t configId = src.ConfigId();
    }

    //auto dataPerConfig = dataContainer2.All(L"Linear Merged");

    {
      const auto cscanData = dataContainer2.Cscan(3);
      const auto& datasets = cscanData->Datasets();
      size_t count = datasets.Count();
      
      if (cscanData->IsDataMerged()) 
      {
        auto cscanDset = datasets.Dataset(1);
        cscanDset = nullptr;
      }
      else
      {
        auto cscanBeamDset = datasets.Dataset(0, 3);
        cscanBeamDset = nullptr;
      }
    }

    {

      const auto ascanData = dataContainer2.Ascan(2);
      const auto& datasets = ascanData->Datasets();
      size_t count = datasets.Count();

      if (ascanData->IsDataMerged())
      {
        auto dataset = datasets.Dataset();
        dataset = nullptr;

      }
      else
      {
        auto dataset = datasets.Dataset(3);
        dataset = nullptr;
      }
    }


    //for (const auto data : dataPerConfig)
    //{
    //  if (auto ascanData = dynamic_cast<const AscanData*>(data)) {
    //    const auto& src = ascanData->Source();
    //    const std::wstring& configName = src.ConfigName();

    //    const auto& datasets = ascanData->Datasets();
    //    size_t count = datasets.Count();
    //    count = 0;
    //  }
    //}

    for (const auto& dataItem : dataContainer2.Items())
    {
      const auto& src = dataItem->Source();
      const std::wstring& configName = src.ConfigName();

      const auto& datasets = dataItem->Datasets();
      for (const auto& ds : datasets.Items())
      {
        if (const auto ascanDset = dynamic_cast<const AscanDataset*>(ds.get())) {
          ReadAscanData(ascanDset);
        }
        else if (const auto cscanDset = dynamic_cast<const CscanDataset*>(ds.get())) {
          ReadCscanData(cscanDset);
        }
      }
    }

    //acquiredData.Close(FILENAME_1);
    ndtData.Close(FILENAME_2);

    //fs.Write();
    //std::vector<int8_t> dataSlice = fs.ReadSliceFpd();
   
    //SaveImage saveImage(dataSlice);

    //CudaKernel ck;
    //FileStorage fs;
    //fs.WriteOneGB();
    //fs.Read(ck);

    //CycleDataDescriptor cdd1;
    //CycleDataDescriptor cdd2;
    //cdd2 = std::move(cdd1);
    //auto p = std::make_unique<Playground>(std::move(cdd1));

    //std::cin.ignore();
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
  }

  return 0;
}

Playground::Playground(CycleDataDescriptor&& cdd) noexcept
  : m_cdd(std::move(cdd))
{
}

Playground& Playground::operator=(Playground&& other) noexcept
{
  if (this != &other)
  {

  }
  return *this;
}

Playground::Playground()
{
  // Log4cplus //

  log4cplus::initialize();


  // ZeroMQ //

  void* context = zmq_ctx_new();


  // RapidJSON //

  const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
  rapidjson::Document d;
  d.Parse(json);
  auto r = d.HasMember("project");


  // Numpy //

  cnpy::NpyArray n;


  // OpenCV //

  cv::Mat mat(100, 200, CV_8UC3);


  // uWebSockets //

  // ws->getUserData returns one of these
  struct PerSocketData {

  };

  // Very simple WebSocket broadcasting echo server
  uWS::App().ws<PerSocketData>("/*", {
    // Settings
    .compression = uWS::DEDICATED_COMPRESSOR_3KB,
    .maxPayloadLength = 16 * 1024 * 1024,
    .idleTimeout = 10,
    .maxBackpressure = 1 * 1024 * 1024,
    // Handlers
    .upgrade = nullptr,
    .open = [](auto* ws) {
      // Let's make every connection subscribe to the "broadcast" topic
      ws->subscribe("broadcast");
    },
    .message = [](auto* ws, std::string_view message, uWS::OpCode opCode) {
      // Exit gracefully if we get a closedown message (ASAN debug)
      if (message == "closedown") {
        // Bye bye
        us_listen_socket_close(0, global_listen_socket);
        ws->close();
      }

      // Simply broadcast every single message we get
      ws->publish("broadcast", message, opCode, true);
    },
    .drain = [](auto* ws) {
    // Check getBufferedAmount here
    },
    .ping = [](auto* ws) {

    },
    .pong = [](auto* ws) {

    },
    .close = [](auto* ws, int code, std::string_view message) {
    // We automatically unsubscribe from any topic here
    }
  }).listen(9001, [](auto* listen_socket) {
    global_listen_socket = listen_socket;
    if (listen_socket) {
      std::cout << "Listening on port " << 9001 << std::endl;
    }
  }).run();


  // Boost //

  std::vector<char> buf(4096);
  boost::asio::io_service ios;
  boost::process::async_pipe ap(ios);
  boost::process::child c(boost::process::search_path("C:\\windows\\system32\\notepad.exe"), boost::process::std_out > ap);

  boost::asio::async_read(ap, boost::asio::buffer(buf),
    [](const boost::system::error_code& ec, std::size_t size) {});

  ios.run();
  int result = c.exit_code();
}

Playground::~Playground()
{
  log4cplus::Logger::shutdown();
}
