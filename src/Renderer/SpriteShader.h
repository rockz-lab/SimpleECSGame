#pragma once
#include "Shader.h"

class SpriteShader : public ShaderBase
{
public:

#pragma region attributes

	ShaderMatrix projection;
	ShaderMatrix model;
	ShaderArray spriteColor;
	ShaderScalar textureSampler;

#pragma endregion

	SpriteShader(const std::string& fileName);

};