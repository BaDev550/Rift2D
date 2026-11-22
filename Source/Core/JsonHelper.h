#pragma once

#include <Core/Handle.h>
#include <Core/UUID.h>
#include <nlohmann/json.hpp>

namespace nlohmann {
	template<typename T>
	void to_json(json& j, const Handle<T>& h) { j = h.index; }

	template<typename T>
	void from_json(const json& j, Handle<T>& h) { h.index = j.get<u64>(); }

	inline void to_json(json& j, const Core::UUID& uuid) { j = static_cast<u64>(uuid); }
	inline void from_json(const json& j, Core::UUID& uuid) { uuid = j.get<u64>(); }
}