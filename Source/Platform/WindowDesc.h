#pragma once

#include <Core/Types.h>
#include <Runtime/PAL/PlatformEvent.h>
#include <string>

namespace PAL {
	using WindowHandle = uptr;
	
	struct WindowDesc {
		u32 width = 800;
		u32 height = 800;
		std::string title = "Window";

		b8 resizable = true;
		b8 minimized = false;
		b8 maximized = true;

		WindowEventCallback eventCallback = nullptr;
	};
}