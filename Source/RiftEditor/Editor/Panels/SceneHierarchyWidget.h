#pragma once
#include <Editor/EditorPanel.h>

RCLASS();
class SceneHierarchyWidget final : public EditorPanel {
	GENERATE_CLASS(SceneHierarchyWidget);
public:
	SceneHierarchyWidget() = default;
	~SceneHierarchyWidget() final = default;

	virtual const char* GetTitle() const final { return "SceneHierarchy"; }
	virtual void OnGUI() final;
private:
	char _searchFilter[128] = {};
};