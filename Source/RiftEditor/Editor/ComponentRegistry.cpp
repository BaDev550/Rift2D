#include "ComponentRegistry.h"
#include <Engine/World/Components/TransformComponent.h>
#include <Engine/World/Components/SpriteComponent.h>
#include <Engine/World/Components/IdenityComponent.h>
#include <Engine/World/Components/PointLightComponent.h>

namespace Rift::Editor {
	void RegisterBuiltinComponents() {
		using namespace Rift::Engine;
		auto& r = EditorComponentRegistry::Get();
		if (!r.All().empty()) return;

		r.Register<IdenityComponent>("IdenityComponent", false);
		r.Register<TransformComponent>("TransformComponent", false);
		r.Register<SpriteComponent>("SpriteComponent");
		r.Register<PointLightComponent>("PointLightComponent");
	}
}