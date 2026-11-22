#pragma once

#include <Core/Types.h>
#include <iostream>
#include <sstream>

static constexpr u64 InvalidHandleId = ~0ull;

template<typename T>
struct Handle {
	u64 index = InvalidHandleId;

	Handle& operator=(const Handle& h) { index = h.index; return *this; }
	b8 operator==(const Handle& h) const { return h.index == index; }
	b8 operator!=(const Handle& h) const { return h.index != index; }

	b8 IsValid() const { return index != InvalidHandleId; }
	explicit operator b8() const { return IsValid(); }
};

template<typename T>
struct GenerationalHandle : public Handle {
	u64 generation = 0;

	b8 IsValid(u64 gen) { return (index != InvalidHandleId) && (generation == gen); }
};

namespace std {
	template<typename T>
	struct hash<Handle<T>> {
		usize operator()(const Handle<T>& handle) const {
			return hash<u64>()(static_cast<u64>(handle.index));
		}
	};
}