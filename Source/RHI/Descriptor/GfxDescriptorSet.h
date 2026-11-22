#pragma once

#include <Core/Types.h>
#include <RHI/Descriptor/GfxDescriptorType.h>
#include <RHI/Descriptor/GfxDescriptorSetAllocInfo.h>

#include <RHI/Object/GfxObject.h>

namespace RHI {
	class GfxImage;
	class GfxBuffer;

	class GfxDescriptorSet : public Object {
	public:
		using Object::Object;
		virtual ~DescriptorSet() = default;
		
		virtual void WriteImage(u32 binding, u32 arrayIndex, DescriptorType type, Image* image) = 0;
		virtual void WriteBuffer(u32 binding, u32 arrayIndex, DescriptorType type, Buffer* buffer) = 0;
	};
}