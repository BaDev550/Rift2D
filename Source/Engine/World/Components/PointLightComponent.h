#pragma once

#include <Engine/World/Components/BaseComponent.h>
#include <Core/Types.h>

namespace Rift::Engine {
	RCLASS();
	class PointLightComponent final : public BaseComponent {
		GENERATE_CLASS(PointLightComponent)
	public:
		RPROP(Edit, Color);
		Runtime::Math::Vec3 color = Runtime::Math::Vec3(1.0f);

		RPROP(Edit);
		f32 intensity = 1.0f;

		RPROP(Edit);
		f32 radius = 5.0f;
	};
}