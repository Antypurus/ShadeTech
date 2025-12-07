#pragma once

#include <core/types.h>

/*
 * Notes
 * ------------------
 *  MacOS requires us to create an NSApplication for the application to process events and handle certain application/client wide events. From there we can then create a NSWindow*|Handle which is what we are going to store for out handles
 */

namespace SHD::Rendering::MacOS {

#define MAX_WINDOW_HANDLES 32

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

enum class WindowEventType{
    resize,
    close,
};

struct WindowEvent{
private:
    struct WindowResizeEventInfo{
        usize new_width;
        usize new_height;
    };
    
public:
    NSWindowHandle window_handle = nullptr;
    WindowEventType event_type;
    union {
        WindowResizeEventInfo resize_event_info;
        bool close_event_info;
    };
};

struct WindowHandle{
    usize handle;
    usize generation;
};

struct Application
{
private:
    NSApplicationHandle m_app = nullptr;
    AppDelegateHandle m_delegate = nullptr;

    NSWindowHandle m_window_handles[MAX_WINDOW_HANDLES];
    bool m_window_open[MAX_WINDOW_HANDLES] = {true};
    usize m_current_window_count = 0;

public:
    Application();
    ~Application();

    void set_activation_policy(ApplicationActivationPolicy policy);
    WindowHandle create_window();
    void run();
    void process_event();
    bool is_open() const;
    
    void pump_event(WindowEvent event);
};

}
