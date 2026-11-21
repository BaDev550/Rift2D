#include "WidgetSubsystem.h"

#include <Engine/Core/Engine.h>
#include <Engine/Reflection/ReflectionSystem.h>
#include <Engine/Graphics/GraphicsSubsystem.h>
#include <Runtime/RHI/ImGui/ImGuiContext.h>
#include <Engine/Platform/PlatformSystem.h>
#include <Engine/Render/RenderFrame.h>
#include <Editor/ComponentRegistry.h>
#include <Editor/EditorPanel.h>

#include <Engine/World/WorldSubsystem.h>
#include <Runtime/Memory/Allocator.h>
#include <Runtime/Definitions/Profiler.h>

#include <imgui.h>

Runtime::Result WidgetSubsystem::OnCreate(Rift::Engine::Engine* engine) {
	Subsystem::OnCreate(engine);
	
	auto* context = _engine->GetSubsystem<Rift::Engine::GraphicsSubsystem>()->GetImGuiContext();
	auto* reflectionSystem = _engine->GetReflectionSystem();

	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(context->GetNativeImGui()));

	auto addWidget = [this](const Runtime::Reflection::Type& type) {
		if (!type.CanConstruct()) return;

		Widget* widget = static_cast<Widget*>(type.Create());
		if (!widget) {
			Logger::Error("WidgetSubsystem", "failed to create a widget instance");
			return;
		}

		const std::string name = widget->GetClassName().data();
		if (_widgetLookupTable.count(name)) {
			Runtime::Mem::Allocator::Destroy(widget);
			return;
		}

		_widgetLookupTable[name] = _widgets.size();
		_widgets.push_back(widget);
		Logger::Info("WidgetSubsystem", "'{}' widget added", name);
		};

	Rift::Editor::RegisterBuiltinComponents();
	reflectionSystem->ForEachWithBase<Widget>(addWidget);
	reflectionSystem->ForEachWithBase<EditorPanel>(addWidget);

	std::stable_sort(_widgets.begin(), _widgets.end(), [](Widget* a, Widget* b) { return a->Order() < b->Order(); });
	_widgetLookupTable.clear();
	for (size_t i = 0; i < _widgets.size(); ++i)
		_widgetLookupTable[_widgets[i]->GetClassName().data()] = i;

	return Runtime::Result();
}

void WidgetSubsystem::OnUpdate() {
	auto system = _engine->GetSubsystem<Rift::Engine::GraphicsSubsystem>();

}

void WidgetSubsystem::OnDestroy() {
	Subsystem::OnDestroy();
	for (auto& widget : _widgets) {
		Runtime::Mem::Allocator::Destroy(widget);
	}
}

void WidgetSubsystem::GetDependencyGraph(Rift::Engine::DependencyGraph& graph) {
	graph.Require<Rift::Engine::GraphicsSubsystem>(Rift::Engine::DependencyOrder::After);
	graph.Require<Rift::Engine::WorldSubsystem>(Rift::Engine::DependencyOrder::After);
}