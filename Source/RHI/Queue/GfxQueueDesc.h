#pragma once

#include <RHI/Queue/GfxQueueType.h>

namespace RHI {
	struct QueueDesc {
		QueueType type = QueueType::Graphics;
	};
}