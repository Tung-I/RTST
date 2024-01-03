#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <chrono>

#include "Utils/conversion.hh"
#include "Utils/udp_socket.hh"
#include "Video/sdl.hh"
#include "protocol.hh"
// #include "vp9_decoder.hh"
#include "HWDecoder.hh"

#include "NvCodecUtils.h"


// simplelogger::Logger *logger = simplelogger::LoggerFactory::CreateConsoleLogger();

void print_usage(const string & program_name)
{
  cerr <<
  "Usage: " << program_name << " [options] host port width height\n\n"
  "Options:\n"
  "--fps <FPS>          frame rate to request from sender (default: 30)\n"
  "--cbr <bitrate>      request CBR from sender\n"
  "--lazy <level>       0: decode and display frames (default)\n"
  "                     1: decode but not display frames\n"
  "                     2: neither decode nor display frames\n"
  "-o, --output <file>  file to output performance results to\n"
  "-v, --verbose        enable more logging for debugging"
  "--streamtime         total streaming time in seconds\n"
  << endl;
}

int main(int argc, char * argv[])
{
  // argument parsing
  uint16_t frame_rate = 30;
  unsigned int target_bitrate = 0; // kbps
  int lazy_level = 0;
  string output_path;
  bool verbose = false;
  uint16_t total_stream_time = 60;

  const option cmd_line_opts[] = {
    {"fps",     required_argument, nullptr, 'F'},
    {"cbr",     required_argument, nullptr, 'C'},
    {"lazy",    required_argument, nullptr, 'L'},
    {"output",  required_argument, nullptr, 'o'},
    {"verbose", no_argument,       nullptr, 'v'},
    {"streamtime", required_argument, nullptr, 'T'},
    { nullptr,  0,                 nullptr,  0 },
  };

  while (true) {
    const int opt = getopt_long(argc, argv, "o:v", cmd_line_opts, nullptr);
    if (opt == -1) {
      break;
    }

    switch (opt) {
      case 'F':
        frame_rate = narrow_cast<uint16_t>(strict_stoi(optarg));
        break;
      case 'C':
        target_bitrate = strict_stoi(optarg);
        break;
      case 'L':
        lazy_level = strict_stoi(optarg);
        break;
      case 'o':
        output_path = optarg;
        break;
      case 'v':
        verbose = true;
        break;
      case 'T':
        total_stream_time = narrow_cast<uint16_t>(strict_stoi(optarg));
        break;
      default:
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }
  }

  if (optind != argc - 4) {
    print_usage(argv[0]);
    return EXIT_FAILURE;
  }

  const string host = argv[optind];
  const auto port = narrow_cast<uint16_t>(strict_stoi(argv[optind + 1]));
  const auto width = narrow_cast<uint16_t>(strict_stoi(argv[optind + 2]));
  const auto height = narrow_cast<uint16_t>(strict_stoi(argv[optind + 3]));

  Address peer_addr_video{host, port};
  UDPSocket video_sock;
  video_sock.connect(peer_addr_video);
  LOG(LogLevel::INFO) << "Video session connected:" << peer_addr_video.str() << ":" << video_sock.local_address().str();

  // create a RTCP socket and connect to the sender
  const auto signal_port = narrow_cast<uint16_t>(port + 1);
  UDPSocket signal_sock;
  Address peer_addr_signal{host, signal_port};
  signal_sock.connect(peer_addr_signal);
  LOG(LogLevel::INFO)<< "Signal session connected" << peer_addr_signal.str() << ":" << signal_sock.local_address().str();

  const ConfigMsg init_config_msg(width, height, frame_rate, target_bitrate); 
  video_sock.send(init_config_msg.serialize_to_string());
  LOG(LogLevel::INFO) <<  "init_config_msg sent";
  const SignalMsg init_signal_msg(target_bitrate); 
  signal_sock.send(init_signal_msg.serialize_to_string());
  LOG(LogLevel::INFO) <<  "init_signal_msg sent";
  
  // Create the decoder
  HWDecoder decoder(width, height, lazy_level, output_path);
  decoder.set_verbose(verbose);

  // Main loop
  const auto start_time = std::chrono::steady_clock::now();
  auto last_time = std::chrono::steady_clock::now();
  while (true) {

    FrameDatagram datagram;
    if (not datagram.parse_from_string(video_sock.recv().value())) {
      throw runtime_error("failed to parse a datagram");
    }

    // Acknowledge the received datagram
    AckMsg ack(datagram);
    video_sock.send(ack.serialize_to_string());
    if (verbose) {
      LOG(LogLevel::INFO) << "Acked datagram: frame_id=" << datagram.frame_id
           << " frag_id=" << datagram.frag_id << endl;
    }


    // Use move() to transfer ownership of dynamically allocated memory
    // After the move operation, the state of datagram is valid but unspecified.
    // Cannot make any assumptions about its content, but can assign a new value to it or destroy it safely.
    decoder.add_datagram(move(datagram));

    while (decoder.next_frame_complete()) {
      decoder.consume_next_frame();
    }

    if (std::chrono::steady_clock::now() - last_time > std::chrono::seconds(1)) {
      // do something every 1s
    }
    last_time = std::chrono::steady_clock::now();

    if (std::chrono::steady_clock::now() - start_time > std::chrono::seconds(total_stream_time)) {
      LOG(LogLevel::INFO) << "Time's up!";
      break;
    }
  }

  return EXIT_SUCCESS;
}