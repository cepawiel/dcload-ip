#pragma once

#include <string.h>
#include <stdint.h>
#include <utility>


namespace dctool{

class PosixSocket;
class Packet {
    friend class PosixSocket;
public:
    static const uint32_t MAX_DATA_LEN = 2048;

    // TODO: Fix this to work properly on all endianness
#define PACKET_KEY_FMT(x) (x[3] << 24 | x[2] << 16 | x[1] << 8 | x[0])
    enum PacketType : uint32_t {
        REBOOT      = PACKET_KEY_FMT("RBOT"),
        
        VERSION     = PACKET_KEY_FMT("VERS"),
        
        MAPLE       = PACKET_KEY_FMT("MAPL"),       

        LOADBIN     = PACKET_KEY_FMT("LBIN"),
        PARTBIN     = PACKET_KEY_FMT("PBIN"),
        DONEBIN     = PACKET_KEY_FMT("DBIN"),
        SENDBIN     = PACKET_KEY_FMT("SBIN"),
        SENDBINQ    = PACKET_KEY_FMT("SBIQ"),

        EXECUTE     = PACKET_KEY_FMT("EXEC"),
        RETURN_VAL  = PACKET_KEY_FMT("RETV"),
        PERF_COUNTS = PACKET_KEY_FMT("PMCR"),
    };
#undef PACKET_KEY_FMT

protected:  
    struct PacketHeader {
        PacketType  Type;
        uint32_t    Address;
        uint32_t    Size;
    } __attribute__ ((packed));

    uint8_t m_InternalPacket[sizeof(PacketHeader) + MAX_DATA_LEN];

    PacketHeader * Header = (PacketHeader *) &m_InternalPacket[0];
    uint8_t * Data = &m_InternalPacket[sizeof(PacketHeader)];
   
    static constexpr size_t GetHeaderSize() { return sizeof(PacketHeader); }
    static constexpr size_t GetMaxSize() { return sizeof(m_InternalPacket); }
};

}