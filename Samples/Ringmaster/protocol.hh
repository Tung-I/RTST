#ifndef PROTOCOL_HH
#define PROTOCOL_HH

#include <string>
#include <memory>
#include <utility> 

enum class FrameType : uint8_t { 
  UNKNOWN = 0, // unknown
  KEY = 1,     // key frame
  NONKEY = 2,  // non-key frame
};

// (frame_id, frag_id)
using SeqNum = std::pair<uint32_t, uint16_t>;

// Base Datagram class
struct BaseDatagram 
{
  BaseDatagram() {}
  BaseDatagram(const uint32_t _frame_id,
               const FrameType _frame_type,
               const uint16_t _frag_id,
               const uint16_t _frag_cnt, 
               const std::string_view _payload);

  virtual ~BaseDatagram() {}

  uint32_t frame_id {};    
  FrameType frame_type {}; 
  uint16_t frag_id {};    
  uint16_t frag_cnt {};  
  uint64_t send_ts {};

  std::string payload {}; 

  // retransmission-related
  unsigned int num_rtx {0};  
  uint64_t last_send_ts {0};  
  

  // serialization and deserialization
  virtual bool parse_from_string(const std::string & binary) = 0;
  virtual std::string serialize_to_string() const = 0;
};

struct FrameDatagram : public BaseDatagram
{
  FrameDatagram() {}
  FrameDatagram(const uint32_t _frame_id,
                const FrameType _frame_type,
                const uint16_t _frag_id,
                const uint16_t _frag_cnt, 
                const uint16_t _frame_width,
                const uint16_t _frame_height,
                const std::string_view _payload);
  
  uint16_t frame_width {};
  uint16_t frame_height {};  
  static const size_t HEADER_SIZE  = sizeof(uint32_t) + 
    sizeof(FrameType) + 4 * sizeof(uint16_t) + sizeof(uint64_t);

  
  static void set_mtu(const size_t mtu);
  static size_t max_payload;

  bool parse_from_string(const std::string & binary) override;
  std::string serialize_to_string() const override;
};

struct TileDatagram : public BaseDatagram
{
  TileDatagram() {}
  TileDatagram(const uint32_t _frame_id,
                const FrameType _frame_type,
                const uint16_t _tile_id,
                const uint16_t _frag_id,
                const uint16_t _frag_cnt, 
                const uint16_t _frame_width,
                const uint16_t _frame_height,
                const std::string_view _payload);
  
  uint16_t tile_id {};
  uint16_t frame_width {};
  uint16_t frame_height {};  
  static const size_t HEADER_SIZE  = sizeof(uint32_t) + 
    sizeof(FrameType) + 5 * sizeof(uint16_t) + sizeof(uint64_t);

  
  static void set_mtu(const size_t mtu);
  static size_t max_payload;

  bool parse_from_string(const std::string & binary) override;
  std::string serialize_to_string() const override;
};

/////////////////////////////////////////////////////////////////////

// Base message class
struct Msg
{
  enum class Type : uint8_t {
    INVALID = 0, 
    ACK = 1,     
    CONFIG = 2,
    SIGNAL = 3  
  };

  Type type {Type::INVALID};

  Msg() {}
  Msg(const Type _type) : type(_type) {} 
  virtual ~Msg() {} // q: what's this syntax? a: virtual destructor

  // factory method to make a (derived class of) Msg
  static std::shared_ptr<Msg> parse_from_string(const std::string & binary);

  // virtual functions for overriding
  virtual size_t serialized_size() const;
  virtual std::string serialize_to_string() const;
};

struct AckMsg : Msg
{
  AckMsg() : Msg(Type::ACK) {}
  AckMsg(const BaseDatagram & datagram);

  uint32_t frame_id {}; 
  uint16_t frag_id {};  
  uint64_t send_ts {};  

  size_t serialized_size() const override; 
  std::string serialize_to_string() const override;
};

struct ConfigMsg : Msg
{
  ConfigMsg() : Msg(Type::CONFIG) {} 
  ConfigMsg(const uint16_t _width, const uint16_t _height,
            const uint16_t _frame_rate, const uint32_t _target_bitrate);  

  uint16_t width {};         
  uint16_t height {};         
  uint16_t frame_rate {};    
  uint32_t target_bitrate {}; 

  size_t serialized_size() const override;
  std::string serialize_to_string() const override;
};

struct SignalMsg : Msg
{
  SignalMsg() : Msg(Type::SIGNAL) {} 
  SignalMsg(const uint32_t _target_bitrate);  
 
  uint32_t target_bitrate {}; 

  size_t serialized_size() const override;
  std::string serialize_to_string() const override;
};

#endif /* PROTOCOL_HH */
