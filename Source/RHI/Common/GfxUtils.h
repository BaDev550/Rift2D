#pragma once

#include <RHI/Buffer/GfxBufferUsage.h>
#include <RHI/Buffer/GfxMemoryUsage.h>

#include <RHI/Image/GfxImageLayout.h>
#include <RHI/Image/GfxImageTileMode.h>
#include <RHI/Image/GfxImageUsage.h>

#include <RHI/Common/GfxFormat.h>

namespace RHI {
	namespace Utils {
		[[nodiscard]] constexpr std::string_view FormatToString(Format format) noexcept {
			switch (format)
			{
			case GfxFormat::Undefined: return "Undefined";
			case GfxFormat::RG8: return "RG8";
			case GfxFormat::RGB8: return "RGB8";
			case GfxFormat::RGBA8: return "RGBA8";
			case GfxFormat::RGBA16: return "RGBA16";
			case GfxFormat::RGBA: return "RGBA";
			case GfxFormat::D32_SFLOAT: return "D32 SFloat";
			case GfxFormat::D24_UNORM_S8_UINT: return "D24 Unorm S8 UInt";
			case GfxFormat::D16_UNORM: return "D16 Unorm";
			case GfxFormat::BC3: return "BC3";
			case GfxFormat::BC5: return "BC5";
			default: return "Unknown format";
			}
		}

		[[nodiscard]] constexpr std::string_view ImageTileModeToString(ImageTileMode tileMode) noexcept {
			switch (tileMode)
			{
			case ImageTileMode::Undefined: return "Undefined";
			case ImageTileMode::Repeat: return "Repeat";
			case ImageTileMode::Mirror: return "Mirror";
			case ImageTileMode::ClampToEdge: return "ClampToEdge";
			case ImageTileMode::ClampToBorder: return "ClampToBorder";
			case ImageTileMode::Optimal: return "Optimal";
			default: return "Unknown tile mode";
			}
		}

		[[nodiscard]] __forceinline const char* ImageUsageToString(ImageUsage usage) noexcept {
			switch (usage)
			{
			case ImageUsage::None: return "None";
			case ImageUsage::Sampled: return "Sampled";
			case ImageUsage::Storage: return "Storage";
			case ImageUsage::ColorAttachment: return "ColorAttachment";
			case ImageUsage::DepthStencilAttachment: return "DepthStencilAttachment";
			case ImageUsage::TransferSrc: return "TransferSrc";
			case ImageUsage::TransferDst: return "TransferDst";
			default: return "Unknown usage";
			}
		}

		[[nodiscard]] __forceinline bool IsFormatDepthStencil(Format format) noexcept {
			switch (format)
			{
			case Format::D24_UNORM_S8_UINT: return true;
			default: return false;
			}
		}

		[[nodiscard]] __forceinline bool IsFormatDepth(Format format) noexcept {
			switch (format)
			{
			case Format::D16_UNORM: return true;
			case Format::D24_UNORM_S8_UINT: return true;
			case Format::D32_SFLOAT: return true;
			default: return false;
			}
		}

		[[nodiscard]] __forceinline u32 GetFormatSize(Format format) noexcept {
			switch (format)
			{
			case Format::Undefined: return 0;
			case Format::RG8: return 2;
			case Format::RGB8: return 3;
			case Format::RGBA8: return 4;
			case Format::RGBA16: return 8;
			case Format::RGBA: return 16;
			default: return 0;
			}
		}
	}
}