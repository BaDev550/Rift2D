#pragma once

#include <Editor/EditorPanel.h>
#include <Editor/ComponentRegistry.h>
#include <memory>
#include <unordered_map>

RCLASS();
class InspectorWidget : public EditorPanel {
	GENERATE_CLASS(InspectorWidget);
public:
	const char* GetTitle() const override { return "Inspector"; }
protected:
	void OnGUI() override;
private:
	void DrawAddComponentPopup(Rift::Engine::Scene& scene, Rift::Engine::EntityHandle entity);
	void CommitPropertyEdits(Rift::Engine::Scene& scene, Rift::Engine::EntityHandle entity);

	bool _wasActive = false;
	Rift::Engine::EntityHandle _lastEntity{ 0xFFFFFFFF, 0 };
	std::unordered_map<std::string, std::shared_ptr<void>> _idle;

	char _componentSearch[64] = {};
};