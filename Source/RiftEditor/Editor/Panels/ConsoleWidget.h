#pragma once
#include <Editor/EditorPanel.h>

RCLASS();
class ConsolleWidget final : public EditorPanel {
	GENERATE_CLASS(ConsolleWidget);
public:
	ConsolleWidget() = default;
	~ConsolleWidget() final = default;

	virtual const char* GetTitle() const final { return "Consolle"; }
	virtual void OnGUI() final;
};