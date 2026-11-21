#pragma once

#include <Runtime/Definitions/Types.h>

namespace Runtime {
	template<typename EnumT>
	class Flags final {
		using UnderlyingEnumT = std::underlying_type_t<EnumT>;
		using DefaultEnumT = static_cast<UnderlyingEnumT>(0);
	public:
		constexpr Flags() : _flags(DefaultEnumT) {}
		constexpr explicit Flags(EnumT value) : _flags(ToUnderlying(value)) {}

		constexpr operator bool() const { return _flags != DefaultEnumT; }
		constexpr Flags operator|(Flags lhs, EnumT rhs) { return Flags(lhs._flags | ToUnderlying(rhs)); }
		constexpr Flags operator|(Flags lhs, Flags rhs) { return Flags(lhs._flags | rhs._flags); }
		constexpr Flags operator&(Flags lhs, EnumT rhs) { return Flags(lhs._flags & ToUnderlying(rhs)); }
		constexpr Flags operator&(Flags lhs, Flags rhs) { return Flags(lhs._flags & rhs._flags); }

		constexpr Flags& operator|=(Flags& lhs, EnumT rhs) {
			lhs._flags |= ToUnderlying(rhs);
			return lhs;
		}
		constexpr Flags& operator|=(Flags& lhs, Flags rhs) {
			lhs._flags |= rhs._flags;
			return lhs;
		}

		constexpr bool operator==(const Flags& lhs, const Flags& rhs) { return lhs._flags == rhs._flags; }
		constexpr bool operator!=(const Flags& lhs, const Flags& rhs) { return lhs._flags != rhs._flags; }
	private:
		static constexpr UnderlyingEnumT ToUnderlying(EnumT tEnum) { return static_cast<UnderlyingEnumT>(tEnum); }
		UnderlyingEnumT _flags;
	};
}