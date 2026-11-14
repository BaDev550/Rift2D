#pragma once

#include <Editor/EditorPanel.h>

RCLASS();
class ViewportPanel : public EditorPanel {
	GENERATE_CLASS(ViewportPanel);
public:
	ViewportPanel() = default;
	const char* GetTitle() const override { return "Viewport"; }

	bool IsHovered() const { return _hovered; }
	bool IsFocused() const { return _focused; }
protected:
	void PushStyle() override;
	void PopStyle() override;

	ImGuiWindowFlags GetFlags() const override { return ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse; }
	void OnGUI() override;
private:
	void DrawToolbar(const ImVec2& origin);

	bool _hovered = false, _focused = false;
};