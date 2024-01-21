#include "dispatcher/dispatcher.h"
#include "channel/udpchannel.h"
#include "protocol/fbprotocol.h"

int main(int argc, char **argv)
{

    FBProtocol protocol;

    UDPChannel channel(protocol, 5001);

    Dispatcher dispatcher;
    dispatcher.reg(channel);

    //loop
    dispatcher.run();

    return 0;
}
