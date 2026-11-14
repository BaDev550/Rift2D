#pragma once

#include <Widget/Widget.h>
#include <Engine/World/ECS/EntityDefinitions.h>

RCLASS();
class DebugOverlayWidget final : public Widget {
	GENERATE_CLASS(DebugOverlayWidget);
public:
	DebugOverlayWidget() = default;
	~DebugOverlayWidget() final = default;

	virtual void Draw() final;
private:
	Rift::Engine::EntityHandle _selectedEntity;
};