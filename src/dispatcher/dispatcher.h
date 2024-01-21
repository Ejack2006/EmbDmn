#pragma once

#include <vector>
#include <functional>
#include "poll.h"

#include "./../channel/channel.h"


class Dispatcher
{
public:

    Dispatcher();
    ~Dispatcher();

    void reg(Channel& channel);
    void unreg(Channel& channel);
    void run();

private:
    std::vector<pollfd> m_fds;
    std::vector<std::reference_wrapper<Channel>> m_chs;
};
