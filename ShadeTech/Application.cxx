module;

#include "Log.h"
#include "Types.h"
#include "assert.h"

#include <iostream>

export module Application;

namespace SHD {

export class Application
{
public:
    Application() = default;
    virtual ~Application() = default;

    template<typename Subclass>
    static i32 RunApplication(Subclass application, u32 argument_count, char** arguments)
    {
        std::cout << "test" << std::endl;
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
