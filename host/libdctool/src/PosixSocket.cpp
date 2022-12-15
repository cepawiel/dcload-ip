#include "PosixSocket.hpp"

#include "Packet.hpp"

using namespace dctool;

PosixSocket::PosixSocket()
: m_Address(""), m_Port(0), m_FD(-1), m_Server({})
{
}

bool PosixSocket::Open(std::string addr, int port)
{
    m_Address= addr;
    m_Port = port;
    m_FD = socket(AF_INET, SOCK_DGRAM, 0);
    if (m_FD < 0)
    {
        printf("Failed to open socket: %d\n", m_FD);
        return false;
    }

    m_Server.sin_family = AF_INET;
    m_Server.sin_port = htons(53535);

    struct hostent* host = gethostbyname(m_Address.c_str());
    if(host == nullptr) {
        printf("gethostbyname returned nullptr!\n");
        return false;
    }

    m_Server.sin_addr = *((struct in_addr *)host->h_addr);
    return true;
};

void PosixSocket::Close()
{
    return;
};

int PosixSocket::Write(uint8_t *data, size_t len)
{
    return sendto(m_FD, data, len, 0,
           (struct sockaddr *) &m_Server, sizeof(struct sockaddr));
};

int PosixSocket::Read(uint8_t *data, size_t len)
{
    struct sockaddr_in server;
    socklen_t s = sizeof(struct sockaddr);
    return recvfrom(m_FD, data, len, 0, 
        (struct sockaddr *) &server, &s);
};

bool PosixSocket::Write(Packet& packet)
{
    auto header = packet.Header;
    auto dataLen = header->Size;
    header->Address = htonl(header->Address);
    header->Size = htonl(dataLen);
    auto packetLen = Packet::GetHeaderSize() + dataLen;
    int len = Write((uint8_t *) header, packetLen);
    return ((uint64_t) len == packetLen);
};

bool PosixSocket::Read(Packet& packet)
{
    auto header = packet.Header;
    int len = Read((uint8_t *) header, Packet::GetMaxSize());
    if ((long unsigned int)len >= Packet::GetHeaderSize()) {
        header->Address = ntohl(header->Address);
        header->Size = ntohl(header->Size);
        return true;
    } else {
        printf("WARN: recvd: %d\n", len);
        return false;
    }
};