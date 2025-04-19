#pragma once

#include <core/platform.h>
#include <core/types.h>
#include <core/utils/log.h>

#if PLATFORM_WINDOWS
#include <core/init/Windows_application.h>
#define PLATFORM_APPLICATION_DERIVE :private Windows::application
#else
#define PLATFORM_APPLICATION_DERIVE
#endif

namespace SHD {

class application PLATFORM_APPLICATION_DERIVE
{
public:
    application() = default;
    virtual ~application() = default;

    template<typename Subclass>
    static i32 run_application(Subclass application, u32 argument_count, char** arguments)
    {
        return application.run(argument_count, arguments);
    };

    virtual i32 run(u32 argument_count, char** arguments) = 0;

public:
    /***************** Deleted Constructors & Operators ********************/
    application(const application& other) = delete;
    application& operator=(const application& other) = delete;
    application(application&& other) = delete;
    application& operator=(application&& other) = delete;
};

}
