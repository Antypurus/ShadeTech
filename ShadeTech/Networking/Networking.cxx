module;

#include "Platform.h"

export module Networking;

#if PLATFORM_WINDOWS
export import windows.networking;
#else
export import posix.networking;
#endif
