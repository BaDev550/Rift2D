#include "ViewportWidget.h"
#include <Engine/Core/Engine.h>
#include <Engine/World/WorldSubsystem.h>
#include <Engine/World/Systems/SceneRenderer2D.h>
#include <imgui.h>
#include <algorithm>

using namespace Rift::Engine;

namespace {
	SceneRenderer2DSystem* GetRenderer() {
		auto* world = Engine::Get()->GetSubsystem<WorldSubsystem>();
		return world ? world->GetSystem<SceneRenderer2DSystem>() : nullptr;
	}
}

void ViewportPanel::PushStyle() { ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0)); }
void ViewportPanel::PopStyle() { ImGui::PopStyleVar(); }

void ViewportPanel::OnGUI() {
	_hovered = ImGui::IsWindowHovered();
	_focused = ImGui::IsWindowFocused();

	const ImVec2 pos = ImGui::GetCursorScreenPos();
	ImVec2 size = ImGui::GetContentRegionAvail();
	size.x = std::max(size.x, 1.0f);
	size.y = std::max(size.y, 1.0f);

	SceneRenderer2DSystem* renderer = GetRenderer();
	if (renderer) {
		renderer->SetViewportSize(static_cast<u32>(size.x), static_cast<u32>(size.y));
	}

	if (renderer && renderer->GetViewportTexture() != ImTextureID_Invalid) {
		ImGui::Image(renderer->GetViewportTexture(), size);
	}
	else {
		ImGui::Dummy(size);
		ImGui::GetWindowDrawList()->AddText(ImVec2(pos.x + 12, pos.y + 40), IM_COL32(160, 160, 160, 255),
			renderer ? "Waiting for the first frame..." : "SceneRenderer2DSystem not found.");
	}
}