#pragma once
#include <Engine/Core/Core.h>

#include <RHI/Shader/ShaderDesc.h>
#include <RHI/Pipeline/GfxVertexAttributeDesc.h>
#include <RHI/Pipeline/GfxPushConstantRange.h>
#include <RHI/Pipeline/GfxPipelineDepth.h>
#include <RHI/Pipeline/GfxPipelineBlending.h>
#include <vector>

namespace RHI {
	struct GfxPipelineDesc {
		ShaderDesc* vertexShader = nullptr;
		ShaderDesc* fragmentShader = nullptr;
		ShaderDesc* computeShader = nullptr;

		GfxDepthTest depthTest = GfxDepthTest::Enabled;
		GfxCompareOp depthCompareOp = GfxCompareOp::Less;
		GfxBlending blending = GfxBlending::Enabled;

		GfxVertexInputDesc inputDesc;
		std::vector<GfxPushConstantRange> pushConstantRanges;

		std::vector<GfxFormat> colorFormats;
		GfxFormat depthFormat;
	};
}