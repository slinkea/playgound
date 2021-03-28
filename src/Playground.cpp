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
#include "Storage/NdtFileReader.hpp"
#include "FileStorage.hpp"
#include "CudaKernels.cuh"


//constexpr char FILENAME[] = "big.h5";
constexpr char FILENAME_1[] = "D:\\NDT Files\\ThinBlade.h5";
constexpr char FILENAME_2[] = "D:\\NDT Files\\4-Configs.h5";


struct us_listen_socket_t* global_listen_socket;

void ReadAscanData(const TAscanDatasetPtr& dset_)
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

  if (const auto ascanBeamDset = dynamic_cast<const AscanBeamDataset*>(dset_.get()))
  {
    size_t idx = ascanBeamDset->BeamIndex();
    idx = 0;
  }
}

void ReadCscanData(const TCscanDatasetPtr& dset_)
{
  const CscanAttributes& attributes = dset_->Attributes();
  const DatasetProperties& props = dset_->Properties();
  const DataDimensions& dims = dset_->Dimensions();

}


int main(int argc, char* argv[])
{
  try
  {
    //FileStorage fs;
    //fs.ReadBig();
    //fs.CompressFile();

    NdtFileReader nfr;

    size_t fileSize1 = H5Utils::Size(FILENAME_1);
    size_t fileSize2 = H5Utils::Size(FILENAME_2);

    nfr.Open(FILENAME_2);

    const auto& dataContainer = nfr.Data();

    const auto ascanDataCfg3 = dataContainer.Ascan(1);
    const auto& ascanSrc = ascanDataCfg3->Source();
    bool ascanBeamMerged = ascanSrc.IsBeamDataMerged();
    const auto& ascanDatasets = ascanDataCfg3->Datasets();
    size_t ascanDsetCount = ascanDatasets.Count();
    const auto beamDset = ascanDatasets.BeamDataset(0);
    size_t ascanBeamIdx = beamDset->BeamIndex();
    const auto& ascanAttr = beamDset->Attributes();

    const auto cscanDataCfg3 = dataContainer.Cscan(4);
    const auto& cscanSrc = cscanDataCfg3->Source();
    bool cscanBeamMerged = cscanSrc.IsBeamDataMerged();
    const auto& cscanDsets = cscanDataCfg3->Datasets();
    size_t cscanDsetCount = ascanDatasets.Count();
    
    const auto gateIDset = cscanDsets.Dataset(0);

    const auto gateIBeamDset = cscanDsets.BeamDataset(0, 0);
    size_t cscanBeamIdx = gateIBeamDset->BeamIndex();
    const auto& cscanAttr = gateIDset->Attributes();

    //

    for (const auto& dataItem : dataContainer.Items())
    {
      const auto& src = dataItem->Source();
      const std::wstring& configName = src.ConfigName();
      size_t configId = src.ConfigId();

      if (const auto ascanData = dynamic_cast<const AscanData*>(dataItem.get()))
      {
        const auto& ascanDsets = ascanData->Datasets();
        for (const auto& ds : ascanDsets.Items()) {
          ReadAscanData(ds);
        }
      }
      else if (const auto cscanData = dynamic_cast<const CscanData*>(dataItem.get()))
      {
        const auto& cscanDsets = cscanData->Datasets();
        for (const auto& ds : cscanDsets.Items()) {
          ReadCscanData(ds);
        }
      }
    }

    nfr.Close();

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
