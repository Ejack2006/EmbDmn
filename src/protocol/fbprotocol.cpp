#include <iostream>
#include <sys/utsname.h>

#include "fbprotocol.h"

FBProtocol::FBProtocol(){}

FBProtocol::~FBProtocol(){}

Cmd FBProtocol::onFrame(const std::string& _addr,
                        const unsigned char *const _inbuf,
                        const size_t _inbuflen,
                        unsigned char *&_outbuf,
                        size_t &_outbuflen)
{
    Cmd res = Cmd::NoCmd;

    if(m_time != 0)//Ждем понг. Нужно проверить не вышло ли время. Если время вышло и это коннект то нужно его обработать
    {
        time_t ntime = time(nullptr);
        if(ntime - m_time > m_timeout)
        {
            m_state = State::Disconnected;
            m_time = 0;
        }
    }
    auto verifier = flatbuffers::Verifier(_inbuf, _inbuflen);
    if(EmbDmn::Protocol::VerifyFrameBuffer(verifier))
    {
        flatbuffers::Offset<EmbDmn::Protocol::Frame> frame;
        if(m_state == State::Connected)
        {
            if(_addr == m_addr)
            {
                switch(EmbDmn::Protocol::GetFrame(_inbuf)->msgtype())
                {
                    case EmbDmn::Protocol::MsgType_Pong:
                        m_time = 0;
                        break;
                    case EmbDmn::Protocol::MsgType_RequestDeviceInfo:
                        if(m_time) break;//если понг еще не приходил ждем его

                        utsname deviceinfo;
                        if(!uname(&deviceinfo))
                        {
                            m_fb.Clear();
                            frame = EmbDmn::Protocol::CreateFrame(m_fb, EmbDmn::Protocol::MsgType_RespondDeviceInfo,
                                                                  EmbDmn::Protocol::Data_deviceinfo,
                                                                  EmbDmn::Protocol::CreateDeviceInfo(m_fb,
                                                                                                     m_fb.CreateString(deviceinfo.nodename),
                                                                                                     m_fb.CreateString(deviceinfo.version),
                                                                                                     m_fb.CreateString("123456"),
                                                                                                     m_fb.CreateString(deviceinfo.machine)).Union());
                            m_fb.Finish(frame);
                            _outbuf = m_fb.GetBufferPointer();
                            _outbuflen = m_fb.GetSize();

                        }
                        else std::clog << "uname failed" << std::endl;
                        break;
                    case EmbDmn::Protocol::MsgType_Disconnect:
                        m_state = State::Disconnected;
                        m_time = 0;
                        break;
                    case EmbDmn::Protocol::MsgType_CloseDaemon:
                        m_state = State::Disconnected;
                        m_time = 0;
                        res = Cmd::CloseDaemon;
                        break;
                    default:
                        break;
                }
            }
        }
        else if(EmbDmn::Protocol::GetFrame(_inbuf)->msgtype() == EmbDmn::Protocol::MsgType_Connect)
        {
            m_addr = _addr;
            m_state = State::Connected;

            m_fb.Clear();
            frame = CreateFrame(m_fb, EmbDmn::Protocol::MsgType_Ping);
            m_fb.Finish(frame);
            _outbuf = m_fb.GetBufferPointer();
            _outbuflen = m_fb.GetSize();

            m_time = time(nullptr);
        }
    }
    else
    {
        std::clog << "Recieved packet is wrong" << std::endl;
    }

    return res;
}
