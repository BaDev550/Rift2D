#pragma once

#include <Engine/Render/RenderItem.h>
#include <Runtime/Definitions/Types.h>
#include <array>

namespace Rift::Engine {
	enum class RenderQueue : u32 {
		None = 0,
		Opaque = BIT(0),
		Transparent = BIT(1),
		Sprite = BIT(2),
		UI = BIT(3),
		Count
	};
	CORE_DEFINE_ENUM_FLAG_OPERATORS(RenderQueue)

	class RenderQueues {
	public:
		void Push(RenderQueue queue, const RenderItem& item) { _items[static_cast<u32>(queue)].push_back(item); }
		void Clear() { for (auto& queue : _items) queue.clear(); }
		std::vector<RenderItem>& Get(RenderQueue queue) { return _items[static_cast<u32>(queue)]; }
		const std::vector<RenderItem>& Get(RenderQueue queue) const { return _items[static_cast<u32>(queue)]; }
	private:
		std::array<std::vector<RenderItem>, static_cast<usize>(RenderQueue::Count)> _items;
	};
}