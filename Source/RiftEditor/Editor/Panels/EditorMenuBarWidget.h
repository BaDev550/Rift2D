#pragma once
#include <Editor/EditorPanel.h>

RCLASS();
class EditorMenuBarWidget final : public EditorPanel {
	GENERATE_CLASS(EditorMenuBarWidget);
public:
	EditorMenuBarWidget() = default;
	~EditorMenuBarWidget() final = default;

	virtual void OnGUI() final;
	virtual ImGuiWindowFlags GetFlags() const { return 
		ImGuiWindowFlags_NoTitleBar | 
		ImGuiWindowFlags_MenuBar | 
		ImGuiWindowFlags_NoBackground | 
		ImGuiWindowFlags_NoDecoration; }

	virtual const char* GetTitle() const final { return "EditorMenuBar"; }
};