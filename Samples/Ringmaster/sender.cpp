#include <getopt.h>
#include <iostream>
#include <string>
#include <memory>
#include <stdexcept>
#include <utility>
#include <chrono>
#include <thread>

#include "Utils/conversion.hh"
#include "Utils/timerfd.hh"
#include "Utils/udp_socket.hh"
#include "Utils/poller.hh"
#include "Video/yuv4mpeg.hh"
#include "protocol.hh"
#include "nv_encoder.hh"
#include "Utils/timestamp.hh"

#include "NvCodecUtils.h"

namespace {
  constexpr unsigned int BILLION = 1000 * 1000 * 1000;
}

simplelogger::Logger *logger = simplelogger::LoggerFactory::CreateConsoleLogger();

void print_usage(const std::string & program_name)
{
  std::cerr <<
  "Usage: " << program_name << " [options] port y4m\n\n"
  "Options:\n"
  "--mtu <MTU>                MTU for deciding UDP payload size\n"
  "-o, --output <file>        file to output performance results to\n"
  "-v, --verbose              enable more logging for debugging"
  << std::endl;
}

std::pair<Address, ConfigMsg> recv_config_msg(UDPSocket & udp_sock)
{
  // wait until a valid ConfigMsg is received
  while (true) {
    const auto & [peer_addr, raw_data] = udp_sock.recvfrom();
    const std::shared_ptr<Msg> msg = Msg::parse_from_string(raw_data.value());
    if (msg == nullptr or msg->type != Msg::Type::CONFIG) {
      std::cerr << "Unknown message type received on video port." << std::endl;
      continue; 
    }
    const auto config_msg = dynamic_pointer_cast<ConfigMsg>(msg);
    if (config_msg) {
      return {peer_addr, *config_msg};
    }
  }
}

std::pair<Address, SignalMsg> recv_signal_msg(UDPSocket & udp_sock)
{
  while (true) {
    const auto & [peer_addr, raw_data] = udp_sock.recvfrom();
    const std::shared_ptr<Msg> msg = Msg::parse_from_string(raw_data.value());
    if (msg == nullptr or msg->type != Msg::Type::SIGNAL) {
      std::cerr << "Unknown message type received on signal port." << std::endl;
      continue; 
    }
    const auto signal_msg = dynamic_pointer_cast<SignalMsg>(msg);
    if (signal_msg) {
      return {peer_addr, *signal_msg};
    }
  }
}

int main(int argc, char * argv[])
{
  // argument parsing
  std::string output_path;
  bool verbose = false;

  const option cmd_line_opts[] = {
    {"mtu",     required_argument, nullptr, 'M'},
    {"output",  required_argument, nullptr, 'o'},
    {"verbose", no_argument,       nullptr, 'v'},
    { nullptr,  0,                 nullptr,  0 },
  };

  while (true) {
    const int opt = getopt_long(argc, argv, "o:v", cmd_line_opts, nullptr);
    if (opt == -1) {
      break;
    }

    switch (opt) {
      case 'M':
        FrameDatagram::set_mtu(strict_stoi(optarg));
        break;
      case 'o':
        output_path = optarg;
        break;
      case 'v':
        verbose = true;
        break;
      default:
        print_usage(argv[0]);
        return std::EXIT_FAILURE;
    }
  }

  if (optind != argc - 2) {
    print_usage(argv[0]);
    return std::EXIT_FAILURE;
  }

  const auto video_port = narrow_cast<uint16_t>(strict_stoi(argv[optind]));
  const auto signal_port = narrow_cast<uint16_t>(video_port + 1);
  const std::string yuv_path = argv[optind + 1];
  UDPSocket video_sock;
  video_sock.bind({"0", video_port});
  std::cerr << "Local address: " << video_sock.local_address().str() << std::endl;
  UDPSocket signal_sock;
  signal_sock.bind({"0", signal_port});
  std::cerr << "Local address: " << signal_sock.local_address().str() << std::endl;

  // Ensure that the receiver is ready to receive the first datagram
  std::cerr << "Waiting for receiver..." << std::endl;
  const auto & [peer_addr_video, init_config_msg] = recv_config_msg(video_sock); 
  std::cerr << "Video stream address: " << peer_addr_video.str() << std::endl;
  video_sock.connect(peer_addr_video);
  const auto & [peer_addr_signal, init_signal_msg] = recv_signal_msg(signal_sock); 
  std::cerr << "Signal stream address: " << peer_addr_signal.str() << std::endl;
  signal_sock.connect(peer_addr_signal);

  // read configuration from the peer
  const auto init_width = init_config_msg.width;
  const auto init_height = init_config_msg.height;
  const auto init_frame_rate = init_config_msg.frame_rate;
  const auto init_target_bitrate = init_config_msg.target_bitrate;

  std::cerr << "Received config: width=" << std::to_string(init_width)
       << " height=" << std::to_string(init_height)
       << " FPS=" << std::to_string(init_frame_rate)
       << " bitrate=" << std::to_string(init_target_bitrate) << std::endl;

  // set UDP socket to non-blocking now
  video_sock.set_blocking(false);
  signal_sock.set_blocking(false);

  // open the video file
  const char * szInFilePath = yuv_path.c_str();
  std::ifstream fpIn(szInFilePath, std::ifstream::in | std::ifstream::binary);
  if (!fpIn)
  {
      std::cout << "Unable to open input file: " << szInFilePath << std::endl;
      return;
  }

  // allocate a frame container
  int nHostFrameSize;
  std::unique_ptr<uint8_t[]> pHostFrame;

  // initialize the encoder
  Encoder encoder(init_width, init_height, init_frame_rate, output_path);
  encoder.set_target_bitrate(init_target_bitrate);
  encoder.set_verbose(verbose);

  // create a periodic timer
  Poller poller;
  Timerfd fps_timer;
  const timespec frame_interval {0, static_cast<long>(BILLION / init_frame_rate)}; // {sec, nsec}
  fps_timer.set_time(frame_interval, frame_interval); // {initial expiration, interval}

  // when the periodic timer fires
  poller.register_event(fps_timer, Poller::In,
    [&]()
    {
      // being lenient: read raw frames 'num_exp' times and use the last one
      const auto num_exp = fps_timer.read_expirations(); 
      if (num_exp > 1) {
        std::cerr << "Warning: skipping " << num_exp - 1 << " raw frames" << std::endl;
      }

      for (unsigned int i = 0; i < num_exp; i++) {
        nRead = fpIn.read(reinterpret_cast<char*>(pHostFrame.get()), nHostFrameSize).gcount(); 
        if !(nRead == nHostFrameSize) // if end of file
        {
          std::cout << "Reach the end of the video file." << std::endl;
          exit(0);
        }    
      }

      // compress 'raw_img' into frame 'frame_id' and packetize it
      encoder.compress_frame(pHostFrame);

      // interested in socket being writable if there are datagrams to send
      if (not encoder.send_buf().empty()) {
        poller.activate(video_sock, Poller::Out);
      }
    }
  );

  // when the video socket is writable
  poller.register_event(video_sock, Poller::Out,
    [&]()
    {
      std::deque<FrameDatagram> & send_buf = encoder.send_buf();

      while (not send_buf.empty()) {
        auto & datagram = send_buf.front();

        // timestamp the sending time before sending
        datagram.send_ts = timestamp_us();

        if (video_sock.send(datagram.serialize_to_string())) {
          if (verbose) {
            std::cerr << "Sent datagram: frame_id=" << datagram.frame_id
                 << " frag_id=" << datagram.frag_id
                 << " frag_cnt=" << datagram.frag_cnt
                 << " rtx=" << datagram.num_rtx << std::endl;
          }

          // move the sent datagram to unacked if not a retransmission
          if (datagram.num_rtx == 0) {
            encoder.add_unacked(std::move(datagram));
          }

          send_buf.pop_front();
        } else { // EWOULDBLOCK; try again later
          datagram.send_ts = 0; // since it wasn't sent successfully
          break;
        }
      }

      // not interested in socket being writable if no datagrams to send
      if (send_buf.empty()) {
        poller.deactivate(video_sock, Poller::Out);
      }
    }
  );

  // when the video socket is readable
  poller.register_event(video_sock, Poller::In,
    [&]()
    {
      while (true) {
        const auto & raw_data = video_sock.recv();

        if (not raw_data) { // EWOULDBLOCK; try again when data is available
          break;
        }
        const std::shared_ptr<Msg> msg = Msg::parse_from_string(*raw_data);

        // ignore invalid or non-ACK messages
        if (msg == nullptr or msg->type != Msg::Type::ACK) {
          return;
        }

        const auto ack = dynamic_pointer_cast<AckMsg>(msg);

        if (verbose) {
          std::cerr << "Received ACK: frame_id=" << ack->frame_id
               << " frag_id=" << ack->frag_id << std::endl;
        }

        // RTT estimation, retransmission, etc.
        encoder.handle_ack(ack);

        // send_buf might contain datagrams to be retransmitted now
        if (not encoder.send_buf().empty()) {
          poller.activate(video_sock, Poller::Out);
        }
      }
    }
  );

  // create a periodic timer for outputting stats every second
  Timerfd stats_timer;
  const timespec stats_interval {1, 0};
  stats_timer.set_time(stats_interval, stats_interval);
  poller.register_event(stats_timer, Poller::In,
    [&]()
    {
      if (stats_timer.read_expirations() == 0) {
        return;
      }
      // output stats every second
      encoder.output_periodic_stats();
    }
  );

  // when the signal socket is readable
  poller.register_event(signal_sock, Poller::In, 
    [&]() 
    {
      while (true) {
        const auto & raw_data = signal_sock.recv();
        if (not raw_data) { // EWOULDBLOCK; try again when data is available
        std:: << "Unknown message type received on RTCP port." << std::endl;
          break;
        }
        const std::shared_ptr<Msg> sig_msg = Msg::parse_from_string(*raw_data);

        // handle the signal message
       if (sig_msg->type == Msg::Type::SIGNAL) {
          const auto signal = dynamic_pointer_cast<SignalMsg>(sig_msg);
          
          std::cerr << "Received signal: bitrate=" << signal->target_bitrate
               << std::endl;
          
          // update the encoder's configuration
    
          encoder.set_target_bitrate(signal->target_bitrate);
        }
        // ignore invalid messages
        return;
      }
    }
  );

  // main loop
  while (true) {
    poller.poll(-1);
  }

  return EXIT_SUCCESS;
}