#include "InspectorWidget.h"
#include <Editor/EditorContext.h>
#include <Editor/EditorCommands.h>
#include <Widget/PropertyDrawer.h>
#include <Engine/Core/Engine.h>
#include <Engine/World/WorldSubsystem.h>
#include <Engine/World/Systems/ScriptingSystem.h>
#include <Engine/World/Components/IdenityComponent.h>
#include <imgui.h>
#include <string>
#include <string_view>

using namespace Rift::Engine;
using namespace Rift::Editor;

void InspectorWidget::OnGUI() {
	auto& ctx = EditorContext::Get();
	auto* worldSystem = Engine::Get()->GetSubsystem<WorldSubsystem>();
	Scene* scenePtr = worldSystem ? worldSystem->GetActiveScene() : nullptr;

	if (!scenePtr || !ctx.selectedEntity.IsValid()) {
		ImGui::TextDisabled("Select an entity in the Scene Outliner to inspect its properties.");
		_wasActive = false;
		_idle.clear();
		return;
	}

	Scene& scene = *scenePtr;
	const EntityHandle entity = ctx.selectedEntity;
	auto* reflection = Engine::Get()->GetReflectionSystem();
	auto& registry = EditorComponentRegistry::Get();

	if (!(_lastEntity == entity)) {
		_lastEntity = entity;
		_wasActive = false;
		_idle.clear();
	}

	if (!_wasActive) {
		for (const auto& info : registry.All()) {
			if (info.has(scene, entity)) _idle[info.name] = info.snapshot(scene, entity);
			else _idle.erase(info.name);
		}
	}

	ImGui::TextDisabled("Entity %u (gen %u)", entity.index, entity.generation);
	if (auto* id = scene.FindComponent<IdenityComponent>(entity)) {
		ImGui::TextDisabled("UUID: %s", id->uuid.Str().c_str());
	}
	ImGui::Separator();

	const EditorComponentInfo* pendingRemove = nullptr;

	for (const auto& info : registry.All()) {
		if (!info.has(scene, entity)) continue;

		ImGui::PushID(info.name.c_str());

		const bool open = ImGui::CollapsingHeader(info.name.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlap);
		if (info.removable) {
			ImGui::SameLine(ImGui::GetContentRegionMax().x - 22.0f);
			if (ImGui::SmallButton("X")) pendingRemove = &info;
		}

		if (open) {
			ImGui::Indent(6.0f);
			if (const auto* type = reflection->GetType(info.name)) {
				DrawReflectedObject(type, info.get(scene, entity));
			}
			else {
				ImGui::TextDisabled("'%s' is not reflected (missing RCLASS?)", info.name.c_str());
			}
			ImGui::Unindent(6.0f);
			ImGui::Spacing();
		}

		ImGui::PopID();
	}

	auto* scriptSystem = worldSystem->GetSystem<ScriptingSystem>();
	if (scriptSystem && scriptSystem->IsScriptable(entity)) {
		if (GameObject* go = scriptSystem->GetGameObject(entity)) {
			ImGui::Separator();
			const auto* scriptType = reflection->GetType(std::string(go->GetClassName()));
			ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "GO: %s", go->GetClassName().data());
			DrawReflectedObject(scriptType, go);
		}
	}

	if (pendingRemove) {
		const EditorComponentInfo info = *pendingRemove;
		auto before = info.snapshot(scene, entity);
		CommandStack::Get().Push("Remove " + info.name,
			[info, &scene, entity]() { if (info.has(scene, entity)) info.remove(scene, entity); },
			[info, &scene, entity, before]() {
				if (before) info.restore(scene, entity, before.get());
				else if (!info.has(scene, entity)) info.add(scene, entity);
			});
		_idle.erase(info.name);
	}

	CommitPropertyEdits(scene, entity);

	ImGui::Spacing();
	ImGui::Separator();
	if (ImGui::Button("+ Add Component", ImVec2(ImGui::GetContentRegionAvail().x, 28.0f))) {
		_componentSearch[0] = '\0';
		ImGui::OpenPopup("AddComponentPopup");
	}
	DrawAddComponentPopup(scene, entity);
}

void InspectorWidget::CommitPropertyEdits(Scene& scene, EntityHandle entity) {
	const bool nowActive = ImGui::IsAnyItemActive();

	if (_wasActive && !nowActive) {
		for (const auto& info : EditorComponentRegistry::Get().All()) {
			auto it = _idle.find(info.name);
			if (it == _idle.end() || !it->second || !info.has(scene, entity)) continue;

			auto after = info.snapshot(scene, entity);
			if (!after || info.equal(it->second.get(), after.get())) continue;

			auto before = it->second;
			const EditorComponentInfo infoCopy = info;
			CommandStack::Get().Push("Edit " + infoCopy.name,
				[infoCopy, &scene, entity, after]() { if (infoCopy.has(scene, entity)) infoCopy.restore(scene, entity, after.get()); },
				[infoCopy, &scene, entity, before]() { if (infoCopy.has(scene, entity)) infoCopy.restore(scene, entity, before.get()); },
				true);
		}
		_idle.clear();
		_wasActive = false;
		return;
	}
	_wasActive = nowActive;
}

void InspectorWidget::DrawAddComponentPopup(Scene& scene, EntityHandle entity) {
	if (!ImGui::BeginPopup("AddComponentPopup")) return;

	ImGui::SetNextItemWidth(220.0f);
	if (ImGui::IsWindowAppearing()) ImGui::SetKeyboardFocusHere();
	ImGui::InputTextWithHint("##CompSearch", "Search...", _componentSearch, sizeof(_componentSearch));
	ImGui::Separator();

	const std::string_view filter(_componentSearch);
	bool any = false;

	for (const auto& info : EditorComponentRegistry::Get().All()) {
		if (info.has(scene, entity)) continue;
		if (!filter.empty() && info.name.find(filter) == std::string::npos) continue;

		any = true;
		if (ImGui::MenuItem(info.name.c_str())) {
			const EditorComponentInfo copy = info;
			CommandStack::Get().Push("Add " + copy.name,
				[copy, &scene, entity]() { if (!copy.has(scene, entity)) copy.add(scene, entity); },
				[copy, &scene, entity]() { if (copy.has(scene, entity)) copy.remove(scene, entity); });
			ImGui::CloseCurrentPopup();
		}
	}

	if (!any) ImGui::TextDisabled("No components available");
	ImGui::EndPopup();
}