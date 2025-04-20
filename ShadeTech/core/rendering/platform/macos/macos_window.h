#pragma once

namespace SHD::Rendering::MacOS {

using AppDelegateHandle = void*;
using NSApplicationHandle = void*;

enum class ApplicationActivationPolicy
{
    Regular, // ordinary app
    Accessory, // application that is not in the dock (not sure what the ramifications are yet)
    Prohibited // headless/daemon app
};

struct Application
{
private:
    NSApplicationHandle m_app = nullptr;
    AppDelegateHandle m_delegate = nullptr;
    
public:
    Application();
    
    void set_activation_policy(ApplicationActivationPolicy policy);
};

}
