#pragma once

#include <RHI/Descriptor/GfxDescriptorHeapDesc.h>
#include <RHI/Descriptor/GfxDescriptorSet.h>
#include <RHI/Object/GfxObject.h>
#include <list>

namespace RHI {
	class GfxDescriptorHeap : public Object {
	public:
		using Object::Object;
		virtual ~DescriptorHeap() = default;

		virtual GfxDescriptorSet* AllocDescriptorSet(const GfxDescriptorSetAllocInfo& allocInfo) = 0;
		
		[[nodiscard]] virtual std::vector<GfxDescriptorSet*>& GetDescriptorSets() noexcept = 0;
		[[nodiscard]] virtual const GfxDescriptorHeapDesc& GetDesc() const noexcept = 0;
	};
}