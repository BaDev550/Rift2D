#pragma once
#include <string>
#include <string_view>

namespace RHI {
	class GfxContext;

	class GfxObject {
	public:
		GfxObject(Context* context) : _context(context) {}
		virtual ~GfxObject() = default;

		virtual void SetObjectDebugName(std::string_view name) { _debugName = name; }
		[[nodiscard]] const std::string& GetObjectDebugName() const { return _debugName; }
	protected:
		template<class T = Context> requires std::is_base_of_v<Context, T>
		inline T* GetOwningRHIContext() { return static_cast<T*>(_context); }

		GfxContext* _context = nullptr;
		std::string _debugName = "";
	};
}