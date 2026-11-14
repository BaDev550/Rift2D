#pragma once

#include <Engine/World/WorldSubsystem.h>
#include <concepts>
#include <cstring>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

namespace Rift::Editor {
	using Rift::Engine::Scene;
	using Rift::Engine::EntityHandle;

	struct EditorComponentInfo {
		std::string name;
		bool removable = true;
		bool snapshotable = false;

		bool  (*has)(Scene&, EntityHandle);
		void* (*get)(Scene&, EntityHandle);
		void  (*add)(Scene&, EntityHandle);
		void  (*remove)(Scene&, EntityHandle);
		std::shared_ptr<void>(*snapshot)(Scene&, EntityHandle);
		void (*restore)(Scene&, EntityHandle, const void*);
		bool (*equal)(const void*, const void*);
	};

	class EditorComponentRegistry {
	public:
		static EditorComponentRegistry& Get() {
			static EditorComponentRegistry instance;
			return instance;
		}

		template<class T>
		void Register(const char* name, bool removable = true) {
			constexpr bool kCopyable = std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T>;

			EditorComponentInfo info;
			info.name = name;
			info.removable = removable;
			info.snapshotable = kCopyable;

			info.has = [](Scene& s, EntityHandle e) { return s.HasComponent<T>(e); };
			info.get = [](Scene& s, EntityHandle e) -> void* { return s.FindComponent<T>(e); };
			info.add = [](Scene& s, EntityHandle e) { s.AddComponent<T>(e); };
			info.remove = [](Scene& s, EntityHandle e) { s.RemoveComponent<T>(e); };

			info.snapshot = [](Scene& s, EntityHandle e) -> std::shared_ptr<void> {
				if constexpr (kCopyable) {
					T* c = s.FindComponent<T>(e);
					return c ? std::shared_ptr<void>(new T(*c), [](void* p) { delete static_cast<T*>(p); }) : nullptr;
				}
				else {
					(void)s; (void)e;
					return nullptr;
				}
				};

			info.restore = [](Scene& s, EntityHandle e, const void* src) {
				if (!s.HasComponent<T>(e)) s.AddComponent<T>(e);
				if constexpr (kCopyable) {
					if (src) *s.FindComponent<T>(e) = *static_cast<const T*>(src);
				}
				else {
					(void)src;
				}
				};

			info.equal = [](const void* a, const void* b) -> bool {
				if constexpr (kCopyable) {
					const T& x = *static_cast<const T*>(a);
					const T& y = *static_cast<const T*>(b);
					if constexpr (requires { { x == y } -> std::convertible_to<bool>; }) return x == y;
					else if constexpr (std::is_trivially_copyable_v<T>) return std::memcmp(&x, &y, sizeof(T)) == 0;
					else return false;
				}
				else {
					(void)a; (void)b;
					return true;
				}
				};

			_components.push_back(std::move(info));
		}

		const std::vector<EditorComponentInfo>& All() const { return _components; }

		const EditorComponentInfo* Find(const std::string& name) const {
			for (auto& c : _components) if (c.name == name) return &c;
			return nullptr;
		}

	private:
		std::vector<EditorComponentInfo> _components;
	};

	void RegisterBuiltinComponents();
}