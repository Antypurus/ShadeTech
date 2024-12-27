module;

#include "Platform.h"

#if PLATFORM_POSIX

#include <sys/socket.h>
#include <arpa/inet.h>

export module posix.socket;
namespace SHD::POSIX::Networking
{

    export class TCPServer
    {
    public:
    };

}

#else

export module posix.socket;

#endif
