#pragma once

#include "./../main.h"

class Channel
{
public:
    virtual Cmd onEvent(short int& _revents) = 0;
    virtual int getfd() = 0;
};
