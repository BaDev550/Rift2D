#pragma once
#include <Engine/Core/Core.h>

#include <RHI/Pipeline/GfxVertexAttributeDesc.h>
#include <RHI/Shader/ShaderDesc.h>
#include <RHI/Pipeline/GfxPushConstantRange.h>
#include <vector>

namespace RHI {
	struct PipelineDesc {
		ShaderDesc* vertexShader = nullptr;
		ShaderDesc* fragmentShader = nullptr;
		ShaderDesc* computeShader = nullptr;

		bool depthTest = true;
		bool blending = true;
		VertexInputDesc inputDesc;

		std::vector<PushConstantRange> pushConstantRanges;

		std::vector<Format> colorFormats;
		Format depthFormat;
	};
}