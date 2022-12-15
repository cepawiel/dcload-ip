#include <stdio.h>

#include "TargetConnection.hpp"
#include "Packet.hpp"
#include <assert.h>
#include <unistd.h>

using namespace dctool;

TargetConnection::TargetConnection() : m_Config(std::nullopt) {
    
}

bool TargetConnection::Setup(TargetConnection::Config config) {
    m_Config = config;

    printf("Connection Name: %s\n", config.Name.c_str());
    printf("Address : Port: %s:%d\n", config.Address.c_str(), config.Port);

    m_Sock = dctool::PosixSocket();
    if(!m_Sock->Open(config.Address, config.Port)) {
        printf("Failed to open Socket\n");
        return false;
    }

    return true;
}

std::optional<std::string> TargetConnection::GetTargetVersion() {
    if( !m_Sock ) return {};

    class VersionPacket : public Packet {
        public:
            VersionPacket() : Packet() { 
                Header->Type = PacketType::VERSION;
                Header->Address = 0;
                Header->Size = 0;
            }

            std::string GetString() {
                std::string s((const char *) Data, Header->Size);
                return s;
            }
    };

    VersionPacket packet;
    Send(packet);
    if( Recv(packet) ){
        return packet.GetString();
    }
    return {};    
}

bool TargetConnection::Reboot() {
    if( !m_Sock ) return {};

    class RebootPacket : public Packet {
        public:
            RebootPacket() : Packet() { 
                Header->Type = PacketType::REBOOT;
                Header->Address = 0;
                Header->Size = 0;
            }
    };

    RebootPacket packet;
    return Send(packet);
}

bool TargetConnection::Execute(uint32_t address) {
    if( !m_Sock ) return false;

    class ExecutePacket : public Packet {
        public:
            ExecutePacket(uint32_t addr) : Packet() { 
                Header->Type = PacketType::EXECUTE;
                Header->Address = addr;
                Header->Size = 0;
            }
    };

    ExecutePacket packet(address);
    Send(packet);
    if( !Recv(packet) ) {
        printf("Failed to start Executing\n");
        return false;
    }
    return true;
}

bool TargetConnection::MemWrite(uint32_t address, uint8_t * data, size_t len) {
    if( !m_Sock ) return false;

    class BinaryLoadStartPacket : public Packet {
        public:
            BinaryLoadStartPacket() : Packet() { 
                Header->Type = PacketType::LOADBIN;
                Header->Address = 0;
                Header->Size = 0;
            }
    };

    class BinaryLoadDataPacket : public Packet {
        public:
            BinaryLoadDataPacket(uint32_t addr, uint8_t * mem, size_t len) : Packet() { 
                Header->Type = PacketType::PARTBIN;
                Header->Address = addr;
                Header->Size = len;
                memcpy(Data, mem, len);
            }
    };

    class BinaryLoadEndPacket : public Packet {
        public:
            BinaryLoadEndPacket() : Packet() { 
                Header->Type = PacketType::DONEBIN;
                Header->Address = 0;
                Header->Size = 0;
            }
    };

    BinaryLoadStartPacket startPacket;
    Send(startPacket);
    if( !Recv(startPacket) ) {
        printf("Failed to send startPacket\n");
        return false;
    }

    uint8_t * currentData = data;

    int dataRemaining = len;
    while(dataRemaining > 0) {
        auto dataSize = (dataRemaining >= BIN_CHUNK_SIZE) ? BIN_CHUNK_SIZE : dataRemaining;
        BinaryLoadDataPacket dataPacket(address, currentData, dataSize);
        Send(dataPacket);
        usleep(1000);
        // No Response Expected
        currentData += BIN_CHUNK_SIZE;
        address += BIN_CHUNK_SIZE;
        dataRemaining -= BIN_CHUNK_SIZE;
    }

    BinaryLoadEndPacket endPacket;
    Send(endPacket);
     if( !Recv(endPacket) ) {
        printf("Failed to send endPacket\n");
        return false;
    }

    return true;
}

bool TargetConnection::MemRead(uint32_t address, uint8_t * data, size_t len) {
    if( !m_Sock ) return false;
    assert(false);
    return false;
}

bool TargetConnection::Send(Packet& packet) {
    return m_Sock->Write(packet);
}

bool TargetConnection::Recv(Packet& packet) {
    return m_Sock->Read(packet);
}

bool TargetConnection::Maple(uint8_t * data, size_t dataLen) {
        if( !m_Sock ) return false;

    class MaplePacket : public Packet {
        public:
            MaplePacket(uint8_t * data, size_t dataLen) : Packet() { 
                Header->Type = PacketType::MAPLE;
                Header->Address = 0;
                assert(((dataLen/4) - 1) == data[3]);
                Header->Size = dataLen;
                if (dataLen > 0) {
                    memcpy(Data, data, dataLen);
                }
            }
    };

    MaplePacket packet(data, dataLen);
    Send(packet);
    if( !Recv(packet) ) {
        printf("Failed to recv maple\n");
        return false;
    }

    return true;
}