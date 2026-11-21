#pragma once

#include <RHI/Descriptor/GfxDescriptorHeapDesc.h>
#include <RHI/Descriptor/GfxDescriptorSet.h>
#include <RHI/Object/GfxObject.h>
#include <list>

namespace RHI {
	class DescriptorHeap : public Object {
	public:
		using Object::Object;
		virtual ~DescriptorHeap() = default;

		virtual DescriptorSet* AllocDescriptorSet(const DescriptorSetAllocInfo& allocInfo) = 0;
		
		[[nodiscard]] virtual std::vector<DescriptorSet*>& GetDescriptorSets() noexcept = 0;
		[[nodiscard]] virtual const DescriptorHeapDesc& GetDesc() const noexcept = 0;
	};
}