#pragma once

#include <Widget/Widget.h>
#include <Core/Types.h>

RCLASS();
class EditorDockspaceWidget : public Widget {
	GENERATE_CLASS(EditorDockspaceWidget);
public:
	i32 Order() const override { return -1000; }
	void Draw() override;

	static void RequestLayoutReset() { s_resetRequested = true; }
private:
	void BuildDefaultLayout(u32 dockId, const struct ImGuiViewport* vp);

	b8 _layoutChecked = false;
	static inline b8 s_resetRequested = false;
};