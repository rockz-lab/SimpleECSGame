#include "Shader.h"

#include <iostream>
#include <fstream>


void ShaderBase::GetUniform(const std::string& name, ShaderVariable& outVariable)
{
	auto cName = name.c_str();
	auto found_it = std::find_if(m_uniforms.begin(), m_uniforms.end(),
		[&name = cName](const ShaderVariable& info) -> bool {return info.name == name; });

	if (found_it != m_uniforms.end())
	{
		outVariable = *found_it;
	}
	else
	{
		outVariable.isValid = false;
		std::cout << "WARNING: Uniform not found: " << name << ". The Uniform was propably optimized out in the shader.\n";
	}
}

void ShaderBase::Bind()
{
	glUseProgram(m_program);
}

void ShaderBase::GetAttribsFromProgram()
{
	glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTES, &m_AttrCount);

	for (GLint i = 0; i < m_AttrCount; i++)
	{
		char name[bufferSize];
		GLsizei length;
		GLenum type;
		GLint size;

		glGetActiveAttrib(m_program, i, bufferSize, &length, &size, &type, name);

		ShaderVariable info { name, length, type, size };
		m_attribs.push_back(info);
	}
}

void ShaderBase::GetUniformsFromProgram()
{
	glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &m_UniformsCount);

	for (GLint i = 0; i < m_UniformsCount; i++)
	{
		char name[bufferSize];
		GLsizei length;
		GLenum type;
		GLint size;

		glGetActiveUniform(m_program, i, bufferSize, &length, &size, &type, name);

		ShaderVariable info{ name, length, type, size, i};
		m_uniforms.push_back(info);
	}
}


// helper method to load the shader text files

GLuint createShader(const std::string& text, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	if (shader == 0)
		std::cerr << "shader failed to create!" << std::endl;

	const GLchar* shaderSourceStrings[1];
	GLint shaderSourceStringLengths[1];

	shaderSourceStrings[0] = text.c_str();              // Source Code conversion to c String
	shaderSourceStringLengths[0] = static_cast<GLsizei>(text.length());

	glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "shader compilation failed:  ");

	return shader;
}

std::string LoadShader(const std::string& fileName)
{
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			std::getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << fileName << "File may not exist.\n";
	}

	return output;
}

// helper method to get error messages
void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}
