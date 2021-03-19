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
#include "AcquiredData.hpp"
#include "FileStorage.hpp"
#include "CudaKernels.cuh"


//constexpr char FILENAME[] = "big.h5";
constexpr char FILENAME[] = "ThinBlade.h5";


struct us_listen_socket_t* global_listen_socket;

int main(int argc, char* argv[])
{
  try
  {
    FileStorage fs;
    //fs.ReadBig();
    //fs.CompressFile();

    auto x = AcquiredData(FILENAME);


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
