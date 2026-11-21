#include "RenderGraph.h"

namespace Rift::Engine {
	void RenderGraph::AddPass(RenderPass* pass) {
		_passes.push_back(pass);
	}

	void RenderGraph::Compile() {
		_nodes.clear();
		_images.clear();

		std::vector<RenderPass*> sorted = _passes;
		std::stable_sort(sorted.begin(), sorted.end(), [](RenderPass* a, RenderPass* b) {
			return a->GetShortingValue() < b->GetShortingValue();
			});

		_nodes.reserve(sorted.size());
		for (RenderPass* pass : sorted) {
			PassNode& node = _nodes.emplace_back();
			node.pass = pass;
			RenderGraphBuilder builder(*this, node);
			pass->Setup(builder);
		}
	}

	void RenderGraph::Execute(const RenderFrame& frame) {
		RenderGraphResourceContainer resources(_images);

		for (PassImage& image : _images) image.layout = Runtime::RHI::ImageLayout::Undefined;

		for (PassNode& node : _nodes) {
			InsertBarriers(frame.cmd, node);
			node.pass->Execute(frame, resources);
		}
	}

	void RenderGraph::InsertBarriers(Runtime::RHI::CommandBuffer* cmd, const PassNode& node)
	{
		auto transition = [&](const ResourceAccess& a) {
			PassImage& img = _images[a.first.index];
			if (img.layout == a.second) return;

			cmd->TransitionImageLayout(img.image, a.second);
			img.layout = a.second;
			};
		for (auto& a : node.reads) transition(a);
		for (auto& a : node.writes) transition(a);
	}

}