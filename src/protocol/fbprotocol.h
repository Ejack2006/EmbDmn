#pragma once

#include <string>
#include <time.h>

#include "protocol.h"
#include "fbschema_generated.h"



class FBProtocol: public Protocol
{
    constexpr static time_t m_timeout = 10;
    constexpr static size_t m_bufSize = 1024;
public:
    FBProtocol();
    ~FBProtocol();

    Cmd onFrame(const std::string& _addr,
                        const unsigned char *const _inbuf,
                        const size_t _inbuflen,
                        unsigned char *&_outbuf,
                        size_t &_outbuflen);

private:
    enum class State
    {
        Disconnected,
        Connected
    };
    time_t m_time{0};//Сохраняем время для засечки таймаута между пинг и понг
    State m_state{State::Disconnected};
    std::string m_addr;//адрес можно было использовать в качестве состояния но тогда нельзя было бы работать с пустыми адресами
    flatbuffers::FlatBufferBuilder m_fb{m_bufSize};
};
