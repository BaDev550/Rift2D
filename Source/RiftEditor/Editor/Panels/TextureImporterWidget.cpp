#include "TextureImporterWidget.h"
#include <Engine/Core/Engine.h>
#include <Engine/Asset/AssetSubsystem.h>
#include <Engine/Asset/Texture/Texture2D.h>
#include <Engine/Asset/Texture/Texture2DStreamer.h>
#include <imgui.h>
#include <filesystem>

namespace Rift::Engine {
	void TextureImporterWidget::Draw() {
		if (!_showModal) return;

		if (!ImGui::Begin("Texture 2D", &_showModal)) {
			ImGui::End();
			return;
		}

		ImGui::Text("Source: %s", _sourcePath.c_str());

		ImGui::SeparatorText("2. Texture Specs");

		const char* formats[] = { "BC3 (DXT5 Compressed)", "BC7 (Normal Map)", "RGBA8 (Uncompressed)" };
		int currentFormat = (_settings.format == Runtime::RHI::Format::BC3) ? 0 : (_settings.format == Runtime::RHI::Format::BC7) ? 1 : 2;
		if (ImGui::Combo("GPU Format", &currentFormat, formats, IM_ARRAYSIZE(formats))) {
			if (currentFormat == 0) _settings.format = Runtime::RHI::Format::BC3;
			else if (currentFormat == 1) _settings.format = Runtime::RHI::Format::BC7;
			else if (currentFormat == 2) _settings.format = Runtime::RHI::Format::RGBA8;
		}

		const char* samplers[] = { "Nearest Clamp (Pixel Art)", "Linear Clamp (Smooth)", "Nearest Repeat", "Linear Repeat" };
		int currentSampler = static_cast<int>(_settings.sampler);
		if (ImGui::Combo("Sampler Filter", &currentSampler, samplers, IM_ARRAYSIZE(samplers))) {
			_settings.sampler = static_cast<Runtime::RHI::StaticSampler>(currentSampler);
		}

		ImGui::Checkbox("Flip Vertically on Load (stbi)", &_settings.flipVertical);

		ImGui::SeparatorText("3. Sprite Sheet Grid Metadata");
		ImGui::Checkbox("Is Sprite Sheet / Atlas", &_settings.isTileMap);
		if (_settings.isTileMap) {
			int cellSize[2] = { static_cast<int>(_settings.tileWidth), static_cast<int>(_settings.tileHeight) };
			if (ImGui::DragInt2("Cell Size (px)", cellSize, 1.0f, 1, 4096)) {
				_settings.tileWidth = static_cast<u32>(cellSize[0]);
				_settings.tileHeight = static_cast<u32>(cellSize[1]);
			}
		}

		ImGui::Spacing();
		if (ImGui::Button("Cook & Import Texture", ImVec2(-1, 32))) {
			auto* assetSubsystem = Engine::Get()->GetSubsystem<AssetSubsystem>();
			Texture2D* importedAsset = assetSubsystem->Get<Texture2D>(_sourcePath, &_settings);
			if (importedAsset) {
				_showModal = false;
				_sourcePath = "";
				_settings = {};
				_statusMessage = "";
			}
		}

		if (!_statusMessage.empty()) {
			ImGui::TextWrapped("%s", _statusMessage.c_str());
		}
		ImGui::End();
	}

	void TextureImporterWidget::OpenImporterModal(const std::string& targetPath) {
		_sourcePath = targetPath;
		_showModal = true;
	}

}