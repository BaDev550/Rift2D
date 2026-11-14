#pragma once
#include <Widget/Widget.h>
#include <Engine/Asset/Asset.h>
#include <Engine/Asset/Texture/Texture2DImportSettings.h>
#include <string>

namespace Rift::Engine {
	RCLASS();
	class TextureImporterWidget final : public Widget {
		GENERATE_CLASS(TextureImporterWidget);
	public:
		TextureImporterWidget() = default;
		~TextureImporterWidget() final = default;

		virtual void Draw() final;

		void OpenImporterModal(const std::string& targetPath);
	private:
		std::string _sourcePath = "Assets/Textures/player_sheet.png";
		std::string _statusMessage;
		TextureImportSettings _settings{};

		b8 _showModal = false;
	};
}