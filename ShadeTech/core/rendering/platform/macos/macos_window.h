#pragma once

namespace SHD::Rendering::MacOS {

using AppDelegateHandle = void*;
using NSApplicationHandle = void*;
using NSWindowHandle = void*;

enum class ApplicationActivationPolicy
{
    Regular,   // ordinary app
    Accessory, // application that is not in the dock (not sure what the ramifications are yet)
    Prohibited // headless/daemon app
};

/*
 note(tiago): apparently macos expects you to run all this code on the main thread or things can start to go
 wrong/UB so i need to think about a design for a MacOS application specialization that will handle this stuff.
 */

struct Application
{
private:
    NSApplicationHandle m_app = nullptr;
    AppDelegateHandle m_delegate = nullptr;
    NSWindowHandle m_window = nullptr;

public:
    Application();
    ~Application();

    void set_activation_policy(ApplicationActivationPolicy policy);
    void create_window();
    void run();
    void process_event();
};

}
