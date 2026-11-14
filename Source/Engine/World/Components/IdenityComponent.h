#pragma once

#include <Engine/World/Components/BaseComponent.h>
#include <Runtime/Definitions/Types.h>
#include <Runtime/Containers/UUID.h>

namespace Rift::Engine {
	RCLASS();
	class IdenityComponent final : public BaseComponent {
		GENERATE_CLASS(IdenityComponent)
	public:
		RPROP();
		std::string name;

		RPROP();
		Runtime::UUID uuid;
	};
}