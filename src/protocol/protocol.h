#pragma once

#include <string>

#include "./../main.h"

class Protocol
{
public:
    virtual Cmd onFrame(const std::string& _addr,
                        const unsigned char *const _inbuf,
                        const size_t _inbuflen,
                        unsigned char *&_outbuf,
                        size_t &_outbuflen) = 0;
};
