#include "Texture2DStreamer.h"

#include <Engine/Core/Engine.h>
#include <Engine/Asset/Asset.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <Engine/Asset/Texture/Utils/ImageCompressor.h>
#include <Engine/Graphics/GraphicsSubsystem.h>
#include <Engine/Platform/PlatformSystem.h>

#include <Runtime/RHI/Buffer/RHIBuffer.h>
#include <Runtime/RHI/Image/RHIImage.h>
#include <Runtime/RHI/Common/RHIUtils.h>
#include <Runtime/Definitions/Profiler.h>
#include <Engine/Asset/Texture/Texture2DImportSettings.h>

#include <Runtime/IO/Path.h>

namespace Rift::Engine {
    Runtime::Result Texture2DStreamer::Cook(AssetSource& source, void* settings) {
        if (source.sourcePath.empty()) {
            return Runtime::Result("No source file provided!");
        }
		auto platform = Engine::Get()->GetPlatformSystem()->GetPlatform();

        TextureImportSettings importSettings;
        if (settings) {
            importSettings = *static_cast<TextureImportSettings*>(settings);
        }

        b8 compressed = importSettings.format != Runtime::RHI::Format::RGB8;
        b8 flipV = importSettings.flipVertical;

        int width, height, comp;
        std::vector<u8> rawData;
        std::vector<u8> imageData;
        platform->MapFile(source.sourcePath, rawData);
        stbi_set_flip_vertically_on_load(flipV);
        u8* rawImageData = stbi_load_from_memory(rawData.data(), (rawData.size() * sizeof(u8)), &width, &height, &comp, 4);
        rawData.clear();
        if (!rawImageData) {
            return Runtime::Result("Failed to load image");
        }

        if (compressed) {
            ImageCompressor::CompressImageToBC3(rawImageData, width, height, imageData);
        }
        else {
			usize imageByteSize = static_cast<usize>(width * height * comp);
			imageData.assign(rawImageData, rawImageData + imageByteSize);
        }

        stbi_image_free(rawImageData);

        Runtime::RHI::ImageDesc imageDesc{};
        imageDesc.format = importSettings.format;
        imageDesc.width = width;
        imageDesc.height = height;
        imageDesc.sampler = importSettings.sampler;
        imageDesc.view.type = Runtime::RHI::ImageViewType::Image2D;
        imageDesc.usage = Runtime::RHI::ImageUsage::Sampled | Runtime::RHI::ImageUsage::TransferDst;
        imageDesc.tileMode = importSettings.tile;

        AssetHeader header{};
        header.magic = TEXTURE2D_MAGIC;
        header.handle = source.handle;
        header.metaSize = sizeof(AssetHeader);
        std::strcpy(header.typeName, source.type.c_str());
        header.dataSize = imageData.size();
        header.dataOffset = (sizeof(AssetHeader) + sizeof(Runtime::RHI::ImageDesc));

        platform->WriteToFile(source.cookedPath, &header, sizeof(AssetHeader));
        platform->WriteToFile(source.cookedPath, &imageDesc, sizeof(Runtime::RHI::ImageDesc), sizeof(AssetHeader));
        platform->WriteToFile(source.cookedPath, imageData.data(), header.dataSize, header.dataOffset);
        Logger::Info("Texture2DCooker", "Texture cooked to loc: {}", std::filesystem::absolute(source.cookedPath).string());

        return Runtime::Result();
    }

    Asset* Texture2DStreamer::Import(AssetSource& source) {
        auto platform = Engine::Get()->GetPlatformSystem()->GetPlatform();

        std::vector<u8> fulldata = platform->ReadFromFile(source.cookedPath);

        u32 magic;
        std::memcpy(&magic, fulldata.data(), sizeof(u32));
        if (magic != TEXTURE2D_MAGIC) {
            Logger::Error("Texture2DImporter", "Not a valid texture asset");
            return nullptr;
        }

        AssetHeader header;
        std::memcpy(&header, fulldata.data(), sizeof(AssetHeader));

        Runtime::RHI::ImageDesc imageDesc{};
        std::memcpy(&imageDesc, (fulldata.data() + sizeof(AssetHeader)), sizeof(Runtime::RHI::ImageDesc));
        
        std::vector<u8> imageData((fulldata.begin() + header.dataOffset), fulldata.end());

        Logger::Info("Texture2DImporter", "Imported Texture info: \n type:{}\n datasize:{}\n dataoffset:{}\n Runtime::UUID:{}\n imageDataSize:{}\n imageWidth:{}\n imageHeight:{}\n imageFormat:{}",
            header.typeName,
            header.dataSize,
            header.dataOffset,
            header.handle.index,
            imageData.size(),
            imageDesc.width,
            imageDesc.height,
            Runtime::RHI::Utils::FormatToString(imageDesc.format));

        Texture2D* texture = Runtime::Mem::Allocator::Construct<Texture2D>(imageDesc);

        texture->SetAssetState(AssetState::Loading);

        auto* graphicsSubsystem = Engine::Get()->GetSubsystem<GraphicsSubsystem>();

        graphicsSubsystem->Submit([=](Runtime::RHI::CommandBuffer* cmd) {
			RHI::Buffer* stagingBuffer = graphicsSubsystem->CreateStagingBuffer(header.dataSize);
			stagingBuffer->Write(imageData.data());

			cmd->TransitionImageLayout(texture->_image, Runtime::RHI::ImageLayout::TransferDst);
			cmd->CopyBufferToImage(stagingBuffer, texture->_image, Runtime::RHI::ImageLayout::TransferDst, imageDesc.width, imageDesc.height);
			cmd->TransitionImageLayout(texture->_image, Runtime::RHI::ImageLayout::ShaderReadOnly);
			texture->SetAssetState(AssetState::Ready);
            Logger::Info("GraphicsSubsystem", "Texture loaded to GPU");
            });

        texture->SetAssetState(AssetState::Loaded);

        return std::move(texture);
    }
}