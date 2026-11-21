#pragma once

#include <RHI/Object/GfxObject.h>
#include <RHI/Buffer/GfxBufferDesc.h>

namespace RHI {
	class GfxBuffer : public Object {
	public:
		using Object::Object;
		virtual ~Buffer() = default;

		virtual void Write(const void* data, usize size = size_max, usize offset = 0) = 0;
		virtual const void* GetMappedPtr() const = 0;

		virtual const usize GetSize() const noexcept = 0;
		virtual const BufferDesc* GetDesc() const = 0;
		virtual const BufferUsage GetUsage() const = 0;
		virtual const MemoryUsage GetMemoryUsage() const = 0;
		virtual const u64 GetGPUAdress() = 0;
		virtual const u32 GetBindlessIndex() = 0;
	};
}