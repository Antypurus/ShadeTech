#include "macos_window.h"

#include <Cocoa/Cocoa.h>
#include <Metal/Metal.h>
#include <QuartzCore/CAMetalLayer.h>

#include <iostream>

@interface MetalView:NSView
@property (nonatomic, strong) CAMetalLayer* metal_layer;
@end
@implementation MetalView
@synthesize metal_layer;
- (instancetype)initWithFrame:(NSRect)frame{
    self = [super initWithFrame:frame];
    if(self)
    {
        self.wantsLayer = YES;
        self.metal_layer = [CAMetalLayer layer];
        self.metal_layer.device = MTLCreateSystemDefaultDevice();
        self.metal_layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
        self.layer = self.metal_layer;
        self.layer.backgroundColor = [[NSColor grayColor] CGColor];
    }
    return self;
}
@end

// MacOS Event handling interface, consider renaming to event handler or something like that
@interface AppDelegate: NSObject<NSApplicationDelegate>
@end
@implementation AppDelegate
- (void)applicationDidFinishLaunching:(NSNotification *)notification {
    std::cout << "Application has ben launched" << std::endl;
}
@end

@interface WindowDelegate : NSObject<NSWindowDelegate>
@property (nonatomic) SHD::Rendering::MacOS::Application* application;
@end
@implementation WindowDelegate
- (void)windowDidResize:(NSNotification*) notification{
    NSWindow* window = notification.object;
    NSSize new_size = window.frame.size;
    std::cout << "window was resized to " << new_size.width << "x" << new_size.height << std::endl;
}

- (void)windowWillClose:(NSNotification *)notification{
    std::cout << "window should now be closed" << std::endl;
    self.application->pump_event(SHD::Rendering::MacOS::WindowEvent{
        .window_handle = notification.object,
    });
}
@end

namespace SHD::Rendering::MacOS
{

Application::Application()
{
    NSApplication* app = [NSApplication sharedApplication];
    this->m_app = (__bridge NSApplicationHandle) app;

    AppDelegate* delegate = [[AppDelegate alloc] init];
    [app setDelegate:delegate];
    this->m_delegate = (__bridge AppDelegateHandle)delegate;
    
}

Application::~Application()
{
    for(usize i = 0; i < this->m_current_window_count; ++i)
    {
        if(this->m_window_handles[i] == nullptr) continue;

        NSWindow* window = (__bridge NSWindow*)this->m_window_handles[i];
        [window release];
    }
    
    AppDelegate* delegate = (__bridge AppDelegate*)this->m_delegate;
    NSApplication* app = (__bridge NSApplication*)this->m_app;
    
    [delegate release];
    [app release];
}

void Application::set_activation_policy(ApplicationActivationPolicy policy)
{
    NSApplication* app = (__bridge NSApplication*)this->m_app;
    switch(policy)
    {
        case(ApplicationActivationPolicy::Regular):
        {
            [app setActivationPolicy:NSApplicationActivationPolicyRegular];
            break;
        }
        case(ApplicationActivationPolicy::Accessory):
        {
            [app setActivationPolicy:NSApplicationActivationPolicyAccessory];
            break;
        }
        case(ApplicationActivationPolicy::Prohibited):
        {
            [app setActivationPolicy:NSApplicationActivationPolicyProhibited];
            break;
        }
    }
}

NSWindowHandle g_window = nullptr;
WindowHandle Application::create_window()
{
    NSRect frame = NSMakeRect(100, 100, 600, 400);
    NSWindow* window = [[NSWindow alloc] initWithContentRect:frame
                                               styleMask:(NSWindowStyleMaskTitled |
                                                          NSWindowStyleMaskResizable |
                                                          NSWindowStyleMaskClosable)
                                                 backing:NSBackingStoreBuffered
                                                   defer:NO];
    
    [window setTitle:@"My First macOS Window"];
    [window makeKeyAndOrderFront:nil];
    
    WindowDelegate* window_delegate = [[WindowDelegate alloc] init];
    window_delegate.application = this;
    [window setDelegate:window_delegate];

    NSWindowHandle handle = (__bridge NSWindowHandle)window;
    if(this->m_current_window_count == 0)
    {
        this->m_window_handles[0] = handle;
    }
    else
    {
        this->m_window_handles[this->m_current_window_count - 1] = handle;
    }
    this->m_current_window_count++;

    return {
        .handle = this->m_current_window_count - 1,
        .generation = 0,
    };
}

void Application::run()
{
    NSApplication* app = (__bridge NSApplication*)this->m_app;
    [app run];
}

void Application::process_event()
{
    NSApplication* app = (__bridge NSApplication*)this->m_app;
    NSEvent *event = [app nextEventMatchingMask:NSEventMaskAny
                                       untilDate:[NSDate dateWithTimeIntervalSinceNow:0.01]
                                          inMode:NSDefaultRunLoopMode
                                         dequeue:YES];
    if(event)
    {
        [app sendEvent:event];
        if(event.type == NSEventTypeLeftMouseDown)
        {
            std::cout << "Left mouse down" << std::endl;
        }
    }
    [app updateWindows];
}

bool Application::is_open() const
{
    NSWindow* window = (__bridge NSWindow*)this->m_window_handles[0];
    return false;
}

}
