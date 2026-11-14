#pragma once

#include <Runtime/Definitions/Types.h>
#include <Runtime/Reflection/ReflectObject.h>

namespace Rift::Engine {
	RCLASS();
	class BaseComponent : public Runtime::Reflection::ReflectObject {
		GENERATE_CLASS(BaseComponent);
	public:
		BaseComponent() = default;
		virtual ~BaseComponent() = default;
		BaseComponent(const BaseComponent&) = delete;
		BaseComponent& operator=(const BaseComponent&) = delete;
		BaseComponent(BaseComponent&&) noexcept = default;
		BaseComponent& operator=(BaseComponent&&) noexcept = default;
	};
}