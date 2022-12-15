#pragma once

#include <string>
#include <optional>
#include <netdb.h>

namespace dctool {

class Packet;
class PosixSocket {
    private:
        std::string m_Address;
        int m_Port;

        int m_FD;
        struct sockaddr_in m_Server;

        int Write(uint8_t *data, size_t len);
        int Read(uint8_t *data, size_t len);

    public:
        PosixSocket();

        bool Open(std::string addr, int port);
        void Close();

        bool Read(Packet &packet);
        bool Write(Packet &packet);
};

}