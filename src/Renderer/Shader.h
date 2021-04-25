#pragma once

#include <gl/glew.h>

#include <string>
#include "ShaderVariables.h"


void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
std::string LoadShader(const std::string& fileName);
GLuint createShader(const std::string& text, GLenum shaderType);


class ShaderBase
{
public:
	ShaderBase() {};

	virtual ~ShaderBase() {};
	
	void Bind();
protected:
	
	void GetUniform(const std::string& name, ShaderVariable& outVariable);

	void GetShaderVariables()
	{
		GetAttribsFromProgram();
		GetUniformsFromProgram();
	};

	
	GLuint m_program;
	GLint m_AttrCount;
	GLint m_UniformsCount;

	std::vector<ShaderVariable> m_attribs;
	std::vector<ShaderVariable> m_uniforms;
	void GetAttribsFromProgram();
	void GetUniformsFromProgram();

	std::vector<GLuint> m_shaders;
private:

};

