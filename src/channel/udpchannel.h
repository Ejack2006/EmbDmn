#pragma once

#include <memory>
#include "sys/socket.h"
#include "netinet/in.h"
#include "unistd.h"

#include "channel.h"
#include "./../protocol/protocol.h"


class UDPChannel: public Channel
{
    constexpr static size_t m_bufSize = 1024;
public:
    UDPChannel(Protocol& _protocol, const int _port);
    ~UDPChannel();

    Cmd onEvent(short int& _revents);
    int getfd();

private:
    int m_sock = socket(AF_INET, SOCK_DGRAM, 0);
    Protocol& m_protocol;
    int m_port;
    std::unique_ptr<unsigned char[]> m_buf = std::unique_ptr<unsigned char[]>(new unsigned char[m_bufSize]);
};
