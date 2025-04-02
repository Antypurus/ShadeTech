#pragma once

#include "Log.h"
#include "Platform.h"
#include "Types.h"

#include <iostream>

#if PLATFORM_WINDOWS
#include "Platform/Windows/WindowsApplication.h"
#define PLATFORM_APPLICATION_DERIVE :private Windows::Application
#else
#define PLATFORM_APPLICATION_DERIVE
#endif

namespace SHD {

class Application PLATFORM_APPLICATION_DERIVE
{
public:
    Application() = default;
    virtual ~Application() = default;

    template<typename Subclass>
    static i32 RunApplication(Subclass application, u32 argument_count, char** arguments)
    {
        return application.Run(argument_count, arguments);
    };

    virtual i32 Run(u32 argument_count, char** arguments) = 0;

public:
    /***************** Deleted Constructors & Operators ********************/
    Application(const Application& other) = delete;
    Application& operator=(const Application& other) = delete;
    Application(Application&& other) = delete;
    Application& operator=(Application&& other) = delete;
};

}
