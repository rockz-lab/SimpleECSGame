#include "SpriteShader.h"

SpriteShader::SpriteShader(const std::string& fileName)
{
	m_program = glCreateProgram();

	auto fileEnding = fileName.find_last_of(".");

	std::string baseName;
	if (fileEnding == std::string::npos)
		baseName = fileName;
	else
		baseName = fileName.substr(fileEnding - 1);

	m_shaders.push_back(createShader(LoadShader(baseName + ".vert"), GL_VERTEX_SHADER));
	m_shaders.push_back(createShader(LoadShader(baseName + ".frag"), GL_FRAGMENT_SHADER));
	
	for (unsigned int i = 0; i < m_shaders.size(); i++)
		glAttachShader(m_program, m_shaders[i]);   

	glLinkProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Error: Pragram failed to link: ");

	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "Error: Pragram invalid: ");
	GetShaderVariables();

	GetUniform("projection", projection);
	GetUniform("model", model);
	GetUniform("spriteColor[0]", spriteColor);
	GetUniform("image", textureSampler);

	textureSampler.Set(GL_TEXTURE0);
}
