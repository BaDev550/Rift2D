#include "EditorMenuBarWidget.h"
#include <Editor/EditorContext.h>
#include <Editor/EditorCommands.h>
#include <Editor/EditorPanel.h>
#include <Editor/EditorDockspaceWidget.h>
#include <Engine/Core/Engine.h>
#include <Widget/WidgetSubsystem.h>
#include <imgui.h>

using namespace Rift::Engine;
using namespace Rift::Editor;

void EditorMenuBarWidget::OnGUI() {
	auto& ctx = EditorContext::Get();
	auto& cmds = CommandStack::Get();
	ImGuiIO& io = ImGui::GetIO();

	if (!io.WantTextInput && io.KeyCtrl) {
		if (ImGui::IsKeyPressed(ImGuiKey_Z)) { if (io.KeyShift) cmds.Redo(); else cmds.Undo(); }
		if (ImGui::IsKeyPressed(ImGuiKey_Y)) cmds.Redo();
	}

	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("New Scene", "Ctrl+N")) {
				ctx.selectedEntity = { 0xFFFFFFFF, 0 };
				cmds.Clear();
			}
			if (ImGui::MenuItem("Open Scene...", "Ctrl+O")) { }
			if (ImGui::MenuItem("Save Scene", "Ctrl+S")) { }
			ImGui::Separator();
			if (ImGui::MenuItem("Exit", "Alt+F4")) Engine::Get()->Shutdown("Requested by editor");
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit")) {
			std::string undoLabel = cmds.CanUndo() ? std::string("Undo ") + cmds.UndoName() : "Undo";
			std::string redoLabel = cmds.CanRedo() ? std::string("Redo ") + cmds.RedoName() : "Redo";
			if (ImGui::MenuItem(undoLabel.c_str(), "Ctrl+Z", false, cmds.CanUndo())) cmds.Undo();
			if (ImGui::MenuItem(redoLabel.c_str(), "Ctrl+Y", false, cmds.CanRedo())) cmds.Redo();
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window")) {
			if (auto* widgets = Engine::Get()->GetSubsystem<WidgetSubsystem>()) {
				for (auto* w : widgets->GetWidgets()) {
					if (auto* panel = dynamic_cast<EditorPanel*>(w)) {
						std::string label(panel->GetClassName().data());
						ImGui::MenuItem(label.c_str(), nullptr, &panel->Open());
					}
				}
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Reset Layout")) EditorDockspaceWidget::RequestLayoutReset();
			ImGui::EndMenu();
		}

		f32 dt = Engine::Get()->GetTime().GetDeltaTime();
		f32 fps = (dt > 0.00001f) ? (1.0f / dt) : 0.0f;
		char fpsText[64];
		snprintf(fpsText, sizeof(fpsText), "%.1f FPS (%.2f ms)", fps, dt * 1000.0f);
		float textWidth = ImGui::CalcTextSize(fpsText).x;
		ImGui::SameLine(ImGui::GetWindowWidth() - textWidth - 20.0f);
		ImGui::TextDisabled("%s", fpsText);

		ImGui::EndMainMenuBar();
	}
}