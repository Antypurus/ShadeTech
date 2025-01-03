module;

#include "Types.h"

import posix.socket;
import Application;

export module ShadeTech;

export class ShadeTech : public SHD::Application
{
public:
<<<<<<< HEAD
    int Run(u32 argument_count, char** arguments) override {
        using namespace SHD::POSIX::Networking; 
        TCPServer server;
        return 0; 
=======
    int Run(u32 argument_count, char** arguments) override
    {
        using namespace SHD::POSIX::Networking;
        TCPServerSocket server;
        return 0;
>>>>>>> 7aa13bc545f1e456887dd9a0c87428a14201dfdc
    }
};
