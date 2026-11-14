#pragma once

#include <string_view>

namespace Runtime::Reflection {
	template<typename>
	struct TypeAccessor;
}

#define RCLASS(...)
#define RSTRUCT(...) // TODO(0x): implement struct reflection support
#define RPROP(...)

#define GENERATE_CLASS(ClassType) \
public: \
	template<typename> \
	friend struct Runtime::Reflection::TypeAccessor; \
	virtual std::string_view GetClassName() override { return #ClassType; } \
	static constexpr std::string_view GetStaticClassName() { return #ClassType; }
