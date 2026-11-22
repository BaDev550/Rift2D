#pragma once
#include <Core/Types.h>
#include <iostream>
#include <random>
#include <sstream>
#include <nlohmann/json.hpp>

static constexpr u64 InvalidUUID = 0;
struct UUID {
public:
	UUID();
	UUID(u64 uuid);
	UUID(std::string_view uuidStr);

	b8 IsValid() const;

	b8 operator==(const UUID& id) const { return id._uuid == _uuid; }
	b8 operator!=(const UUID& id) const { return id._uuid != _uuid; }
	explicit operator b8() const { return IsValid(); }
	operator u64() const { return _uuid; }

	std::string Str() const;
private:
	u64 _uuid = InvalidUUID;
};

namespace std {
	template<>
	struct hash<UUID> {
		size_t operator()(const UUID& uuid) const {
			return hash<u64>()(static_cast<u64>(uuid));
		}
	};
}