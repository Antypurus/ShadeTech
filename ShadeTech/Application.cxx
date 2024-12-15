module;

#include "Log.h"
#include "Types.h"
#include "assert.h"

import socket;
using namespace SHD::Windows;

export module Application;

namespace SHD {

export class Application
{
private:
    bool m_running = false;

public:
    Application(u32 argumentCount, char** arguments);
    ~Application();
    u32 Run();

    void Init()
    {
        TCPServer socket;
        socket.listen();
    };

    void Update() { this->m_running = false; };

    void Shutdown() {};
};

Application::Application(u32 argumentCount, char** arguments)
{
    this->Init();
}

u32 Application::Run()
{
    this->m_running = true;
    while (this->m_running) {
        this->Update();
    }
    return 0;
}

Application::~Application()
{
    this->Shutdown();
}

export u32 RunApplication(u32 argumentCount, char** arguments)
{
    Application app(argumentCount, arguments);
    return app.Run();
}

}
