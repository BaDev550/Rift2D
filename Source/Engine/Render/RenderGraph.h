#pragma once

#include <Engine/Render/RenderPass.h>
#include <Engine/Render/RenderGraphBuilder.h>
#include <Runtime/RHI/Command/RHICommandBuffer.h>
#include <algorithm>
#include <vector>

namespace Rift::Engine {
	struct PassImage {
		std::string name;
		Runtime::RHI::Image* image = nullptr;
		Runtime::RHI::ImageLayout layout = Runtime::RHI::ImageLayout::Undefined;
		b8 imported = false;
	};

	struct PassNode {
		RenderPass* pass = nullptr;
		std::vector<ResourceAccess> reads;
		std::vector<ResourceAccess> writes;
	};

	class RenderGraphResourceContainer final {
	public:
		explicit RenderGraphResourceContainer(const std::vector<PassImage>& images) : _images(images) {}
		Runtime::RHI::Image* GetImage(ResourceHandle handle) const { return _images[handle.index].image; }
	private:
		const std::vector<PassImage>& _images;
	};

	class RenderGraph final {
	public:
		void AddPass(RenderPass* pass);
		void Compile();
		void Execute(const RenderFrame& frame);
	private:
		void InsertBarriers(Runtime::RHI::CommandBuffer* cmd, const PassNode& node);

		std::vector<RenderPass*> _passes;
		std::vector<PassNode> _nodes;
		std::vector<PassImage> _images;

		friend class RenderGraphBuilder;
	};
}