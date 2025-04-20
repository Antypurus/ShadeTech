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
    }
    return self;
}
@end

@interface AppDelegate: NSObject<NSApplicationDelegate>
@end

@implementation AppDelegate
@end

namespace SHD::Rendering::MacOS
{

Application::Application()
{
    NSApplication* app = [NSApplication sharedApplication];
    this->m_app = (__bridge NSApplicationHandle)app;

    AppDelegate* delegate = [[AppDelegate alloc] init];
    this->m_delegate = (__bridge AppDelegateHandle)delegate;
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

}
