#include "RenderGraphBuilder.h"
#include "RenderGraph.h"

namespace Rift::Engine {
	ResourceHandle RenderGraphBuilder::Import(std::string_view name, Runtime::RHI::Image* image) {
		PassImage passImage{};
		passImage.name = name.data();
		passImage.image = image;
		passImage.imported = true;
		u32 index = _graph._images.size();
		_graph._images.push_back(std::move(passImage));
		return { index };
	}	

	ResourceHandle RenderGraphBuilder::Read(ResourceHandle handle, Runtime::RHI::ImageLayout usage) {
		_node.reads.push_back({ handle, usage });
		return handle;
	}

	ResourceHandle RenderGraphBuilder::Write(ResourceHandle handle, Runtime::RHI::ImageLayout usage) {
		_node.writes.push_back({ handle, usage });
		return handle;
	}

	ResourceHandle RenderGraphBuilder::Find(std::string_view name) const {
		for (u32 i = 0; i < _graph._images.size(); i++) {
			if (_graph._images[i].name == name) return { i };
		}
		return {};
	}
}