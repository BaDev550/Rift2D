#include <Engine/Core/Engine.h>

#include <Engine/Graphics/GraphicsSubsystem.h>
#include <Engine/Platform/Window/WindowSubsystem.h>
#include <Engine/Asset/AssetSubsystem.h>
#include <Engine/World/WorldSubsystem.h>
#include <Engine/Input/InputSubsystem.h>
#include <Widget/WidgetSubsystem.h>

#include <reflectManifest.h>

int main() {
	{
		Rift::Engine::Engine engine{};
		engine.AddSubsystem<Rift::Engine::WindowSubsystem>();
		engine.AddSubsystem<Rift::Engine::InputSubsystem>();
		engine.AddSubsystem<Rift::Engine::AssetSubsystem>();
		engine.AddSubsystem<Rift::Engine::GraphicsSubsystem>();
		engine.AddSubsystem<Rift::Engine::WorldSubsystem>();
		engine.AddSubsystem<WidgetSubsystem>();
		engine.Run();
	}
	return 0;
}