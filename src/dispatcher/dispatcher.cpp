#include <iostream>

#include "dispatcher.h"
#include "./../main.h"

Dispatcher::Dispatcher(){}

Dispatcher::~Dispatcher(){}

void Dispatcher::reg(Channel& channel)
{
    m_fds.push_back({channel.getfd(),POLLIN,0});
    m_chs.push_back(std::reference_wrapper<Channel>(channel));
}
void Dispatcher::unreg(Channel& channel)
{
    for(int i = 0; i < m_chs.size(); i++)
        {
            if(&m_chs[i].get() == &channel)
            {
                m_fds.erase(m_fds.begin()+i);
                m_chs.erase(m_chs.begin()+i);
                break;
            }
        }
}
void Dispatcher::run()
{
    while(true)
    {
        if(poll(m_fds.data(), m_fds.size(), -1) == -1)
        {
            std::clog << "poll failed" << std::endl;
            return;
        }
        for(int i = 0; i < m_fds.size(); i++)
        {
            if(m_fds[i].revents)
            {
                if(m_chs[i].get().onEvent(m_fds[i].revents) == Cmd::CloseDaemon)
                {
                    std::clog << "CloseDaemon command received" << std::endl;
                    return;
                }
            }
        }
    }
}
