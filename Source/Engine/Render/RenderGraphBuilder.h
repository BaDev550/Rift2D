#pragma once

#include <Runtime/RHI/Image/RHIImageUsage.h>
#include <Runtime/RHI/Image/RHIImage.h>
#include <Runtime/Definitions/Handle.h>
#include <string_view>

namespace Rift::Engine {
	struct PassNode;
	class RenderGraph;

	using ResourceHandle = Runtime::Handle<Runtime::RHI::Image>;
	using ResourceAccess = std::pair<ResourceHandle, Runtime::RHI::ImageLayout>;

	class RenderGraphBuilder final {
	public:
		RenderGraphBuilder(RenderGraph& graph, PassNode& node) : _graph(graph), _node(node) {}

		ResourceHandle Import(std::string_view name, Runtime::RHI::Image* image);
		ResourceHandle Read(ResourceHandle handle, Runtime::RHI::ImageLayout usage = Runtime::RHI::ImageLayout::ShaderReadOnly);
		ResourceHandle Write(ResourceHandle handle, Runtime::RHI::ImageLayout usage = Runtime::RHI::ImageLayout::ColorAttachment);
		ResourceHandle Find(std::string_view name) const;
	private:
		RenderGraph& _graph;
		PassNode& _node;
	};
}