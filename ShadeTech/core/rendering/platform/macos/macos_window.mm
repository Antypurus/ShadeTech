#include "macos_window.h"

#include <Cocoa/Cocoa.h>
#include <Metal/Metal.h>
#include <QuartzCore/CAMetalLayer.h>

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
}
@end

namespace SHD::Rendering::MacOS
{

Application::Application()
{
    NSApplication* app = [NSApplication sharedApplication];
    this->m_app = (__bridge NSApplicationHandle) app;

    AppDelegate* delegate = [[AppDelegate alloc] init];
    this->m_delegate = (__bridge AppDelegateHandle)delegate;
    [app setDelegate:delegate];
    //[app run];
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
void Application::create_window()
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

    g_window = (__bridge NSWindowHandle)window;
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
    }
    [app updateWindows];
}

}
