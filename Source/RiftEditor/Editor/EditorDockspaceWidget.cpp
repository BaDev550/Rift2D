#include "EditorDockspaceWidget.h"
#include <imgui.h>
#include <imgui_internal.h>

void EditorDockspaceWidget::Draw() {
	ImGuiViewport* vp = ImGui::GetMainViewport();

	ImGuiID dockId = ImGui::DockSpaceOverViewport(0, vp, ImGuiDockNodeFlags_None);

	if (s_resetRequested || !_layoutChecked) {
		ImGuiDockNode* node = ImGui::DockBuilderGetNode(dockId);
		if (s_resetRequested || !node || !node->IsSplitNode()) {
			BuildDefaultLayout(dockId, vp);
		}
		_layoutChecked = true;
		s_resetRequested = false;
	}
}

void EditorDockspaceWidget::BuildDefaultLayout(unsigned int dockId, const ImGuiViewport* vp) {
	ImGui::DockBuilderRemoveNode(dockId);
	ImGui::DockBuilderAddNode(dockId, ImGuiDockNodeFlags_DockSpace);
	ImGui::DockBuilderSetNodeSize(dockId, vp->WorkSize);

	ImGuiID center = dockId;
	ImGuiID left = ImGui::DockBuilderSplitNode(center, ImGuiDir_Left, 0.20f, nullptr, &center);
	ImGuiID right = ImGui::DockBuilderSplitNode(center, ImGuiDir_Right, 0.26f, nullptr, &center);
	ImGuiID bottom = ImGui::DockBuilderSplitNode(center, ImGuiDir_Down, 0.30f, nullptr, &center);

	ImGui::DockBuilderDockWindow("Scene Outliner", left);
	ImGui::DockBuilderDockWindow("Inspector", right);
	ImGui::DockBuilderDockWindow("Asset Browser", bottom);
	ImGui::DockBuilderDockWindow("Console", bottom);
	ImGui::DockBuilderDockWindow("Performance & Stats", right);
	ImGui::DockBuilderDockWindow("Viewport", center);
	ImGui::DockBuilderFinish(dockId);
}