#pragma once
#include <Runtime/PAL/Window/WindowDesc.h>
#include <Core/Types.h>
#include <string>

namespace PAL {
	class Window {
	public:
		Window();
		~Window();

		virtual b8 ShouldClose() const;
		virtual void PollEvents();
		[[nodiscard]] virtual WindowHandle GetHandle() const;
		[[nodiscard]] virtual WindowDesc& GetDesc();
		[[nodiscard]] virtual const u32 GetWidth() const;
		[[nodiscard]] virtual const u32 GetHeight() const;
		[[nodiscard]] virtual const b8 IsMinimized() const;
	};
}