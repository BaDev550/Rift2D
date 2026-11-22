#pragma once

#include <RHI/Object/GfxObject.h>

namespace RHI {
	class GfxFence : public GfxObject {
	public:
		using GfxObject::GfxObject;
		virtual ~GfxFence() = default;

		virtual void Wait(u64 value) = 0;
		virtual void Signal(u64 value) = 0;
		virtual u64 GetCurrentValue() = 0;
	};
}