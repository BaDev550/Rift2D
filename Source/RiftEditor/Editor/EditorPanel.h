#pragma once

#include <Widget/Widget.h>
#include <imgui.h>

RCLASS();
class EditorPanel : public Widget {
	GENERATE_CLASS(EditorPanel);
public:
	EditorPanel() = default;

	void Draw() final {
		if (!_open) return;
		if (ImGui::Begin(GetTitle(), &_open, GetFlags())) {
			PushStyle();
			OnGUI();
			PopStyle();
		}
		ImGui::End();
	}
	bool& Open() { return _open; }
protected:
	virtual const char* GetTitle() const = 0;
	virtual void PushStyle() {}
	virtual void PopStyle() {}
	virtual ImGuiWindowFlags GetFlags() const { return 0; }
	virtual void OnGUI() = 0;
	bool _open = true;
};