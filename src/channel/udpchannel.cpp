#include <iostream>
#include <stdexcept>

#include "udpchannel.h"

UDPChannel::UDPChannel(Protocol& _protocol, const int _port)
:m_protocol(_protocol)
,m_port(_port)
{
	if(m_sock == -1)
	{
		std::clog << "socket failed" << std::endl;
		throw std::runtime_error("socket failed");
	}
    sockaddr_in sa{};
	sa.sin_family = AF_INET;
	sa.sin_port = htons(m_port);
	sa.sin_addr.s_addr = INADDR_ANY;
	bind(m_sock, (sockaddr*)&sa, sizeof(sa));
	if(m_sock == -1)
	{
		std::clog << "bind failed" << std::endl;
		throw std::runtime_error("bind failed");
	}
}

UDPChannel::~UDPChannel()
{
	shutdown(m_sock, SHUT_RDWR);
    close(m_sock);
}

Cmd UDPChannel::onEvent(short int& _revents)
{
	Cmd res = Cmd::NoCmd;

    _revents = 0;
	sockaddr_in ra{};
	socklen_t iralenght = sizeof(sockaddr_in);

    size_t len = recvfrom(m_sock, m_buf.get(), m_bufSize, 0, (sockaddr*)&ra, &iralenght);
	if(len != -1)
	{
		std::string addr = std::to_string((ra.sin_addr.s_addr&0x000000FF)>>0);
		addr += ".";
		addr += std::to_string((ra.sin_addr.s_addr&0x0000FF00)>>8);
		addr += ".";
		addr += std::to_string((ra.sin_addr.s_addr&0x00FF0000)>>16);
		addr += ".";
		addr += std::to_string((ra.sin_addr.s_addr&0xFF000000)>>24);
		addr += ":";
		addr += std::to_string(ntohs(ra.sin_port));

		unsigned char *outbuf;
		size_t outbuflen = 0;
		res = m_protocol.onFrame(addr, m_buf.get(), len, outbuf, outbuflen);
		if(outbuflen)
		{
			//ra.sin_port = htons(m_port);
			len = sendto(m_sock, outbuf, outbuflen, 0, (sockaddr*)&ra, iralenght);
			if(len == -1) std::clog << "sendto failed" << std::endl;
		}
	}
	else std::clog << "recvfrom failed" << std::endl;
	return res;
}
int UDPChannel::getfd()
{
	return m_sock;
}
