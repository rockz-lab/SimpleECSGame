#pragma once
#include <array>
#include <cassert>
#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

static const int bufferSize = 15;

struct ShaderVariable
{
	//ShaderVariable() = default;
	std::string name = "";
	GLsizei length;
	GLenum type;
	GLint size;

	GLint location;

	bool isValid = true;

	bool Validate()
	{
		if (isValid == false)
		{
			std::cout << "Warning: Uniform is not set as it was not found!" << std::endl;
			return false;
		}
		else
			return true;
	}
};


template<class T> struct dependent_false : std::false_type {};

class ShaderScalar : public ShaderVariable
{
public:
	ShaderScalar() {};
	// construct from base class
	ShaderScalar(const ShaderVariable& base) : ShaderVariable{ base } {
		assert(type == GL_FLOAT || type == GL_DOUBLE || type == GL_INT);
	}

	template <typename T>
	void Set(T scalar)
	{
#ifdef _DEBUG
			Validate();
#endif
		if constexpr (std::is_same<T, float>())
			glUniform1f(location, scalar);
		else if constexpr (std::is_same<T, double>())
			glUniform1d(location, scalar);
		else if constexpr (std::is_same<T, int>())
			glUniform1i(location, scalar);
		else
			static_assert(dependent_false<T>(), "This type is not supported for scalar uniforms");
	}
};

class ShaderArray : public ShaderVariable
{
public:
	ShaderArray() {};
	// construct from base class
	ShaderArray(const ShaderVariable& base) : ShaderVariable{ base } {
		assert(type == GL_FLOAT || type == GL_DOUBLE || type == GL_INT);
	}

	template <typename T>
	void Set(const T* data, size_t length)
	{
#ifdef _DEBUG
			if (!Validate()) return;
#endif	
		assert(this->size == length);

		if constexpr (std::is_same<T, float>())
			glUniform1fv(location, this->size, data);
		else if constexpr (std::is_same<T, double>())
			glUniform1dv(location, this->size, data);
		else if constexpr (std::is_same<T, int>())
			glUniform1iv(location, this->size, data);
		else
			static_assert(dependent_false<T>(), "This type is not supported for scalar uniforms");
	}

	template <typename T>
	void Set(const std::vector<T>& data)
	{
		Set(data.data(), data.size());
	}

	template <typename T, int N>
	void Set(const std::array<T, N>& data)
	{
		Set(data.data(), data.size());
	}
};

class ShaderMatrix : public ShaderVariable
{
public:
	ShaderMatrix() {}
	// construct from base class
	ShaderMatrix(const ShaderVariable& base) : ShaderVariable{ base } {
		assert(type == GL_FLOAT_MAT3 || type == GL_DOUBLE_MAT3);
	}
	template <typename T>
	void Set(const T& data)
	{
#ifdef _DEBUG
			Validate();
#endif _DEBUG

		if constexpr (std::is_same<T, glm::mat2>())
			glUniformMatrix2fv(location, 1, false, glm::value_ptr(data));
		else if constexpr (std::is_same<T, glm::mat3>())
			glUniformMatrix3fv(location,  1, false, glm::value_ptr(data));
		else if constexpr (std::is_same<T, glm::mat4>())
			glUniformMatrix4fv(location, 1, false, glm::value_ptr(data));
		else
			static_assert(dependent_false<T>(), "This type is not supported for scalar uniforms");
	}

};