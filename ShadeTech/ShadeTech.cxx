module;

#include "Types.h"

export module ShadeTech;

export import Application;
export class ShadeTech : public SHD::Application
{
public:
    int Run(u32 argument_count, char** arguments) override { 
        return 0; 
    }
};

