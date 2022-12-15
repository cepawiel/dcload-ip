#pragma once

#include <string>
#include <optional>

#include "PosixSocket.hpp"
#include "Packet.hpp"

namespace dctool {



class TargetConnection {
protected:
    static const int BIN_CHUNK_SIZE = 1440;
public:
    // enum Logging { NONE, ERROR, WARN, INFO, DEBUG };

    struct Config {
    public:
        std::string Name;
        std::string Address;
        int Port = 53535;

        // Logging LogLevel = INFO;
    };

private:
    std::optional<Config> m_Config;
    std::optional<PosixSocket> m_Sock;

public:

    TargetConnection();
    bool Setup(Config config);

    std::optional<std::string> GetTargetVersion();
    bool Reboot();
    bool Execute(uint32_t address);
    bool MemWrite(uint32_t address, uint8_t * data, size_t len);
    bool MemRead(uint32_t address, uint8_t * data, size_t len);

    bool Maple(uint8_t * data, size_t dataLen);

private:
    bool Send(Packet&);
    bool Recv(Packet&);

};
    
}