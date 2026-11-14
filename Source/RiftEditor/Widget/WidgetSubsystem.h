#pragma once

#include <Engine/Subsystem/Subsystem.h>
#include <Widget/Widget.h>
#include <unordered_map>

class WidgetSubsystem final : public Rift::Engine::Subsystem {
public:
	virtual Runtime::Result OnCreate(Rift::Engine::Engine* engine) final;
	virtual void OnUpdate() final;
	virtual void OnDestroy() final;

	virtual void GetDependencyGraph(Rift::Engine::DependencyGraph& graph) final;

	const std::vector<Widget*>& GetWidgets() const { return _widgets; }

	template<typename T> requires std::is_base_of_v<Widget, T>
	T* GetWidget() {
		std::string className(T::GetStaticClassName());
		if (auto it = _widgetLookupTable.find(className); it != _widgetLookupTable.end()) {
			return static_cast<T*>(_widgets[it->second]);
		}
		return nullptr;
	}
private:
	std::unordered_map<std::string, i32> _widgetLookupTable;
	std::vector<Widget*> _widgets;
};