#pragma once
#include <Engine/Core/Core.h>
#include <Runtime/PAL/Window/Window.h>
#include <Runtime/PAL/PlatformContext.h>

namespace RHI {
	struct GfxContextDesc {
		struct GfxFeatures {
			bool dynamicRendering = false;
			bool synchronization2 = false;
			bool timelineSemaphore = false;
			bool bufferDeviceAddress = false;
			bool descriptorIndexing = false;
			bool runtimeDescriptorArray = false;
			bool robustBufferAccess = false;
			bool samplerAnisotropy = false;
			bool rayTracing = false;
			bool meshShader = false;
		} features;
		b8 enableValidationLayer = false;

		struct PlatformDesc {
			Runtime::PAL::WindowHandle windowHandle;
			Runtime::PAL::Platform platform;
		} platformDesc;
	};
}