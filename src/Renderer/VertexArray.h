#pragma once
#include "Vertex.h"

#include <vector>

#include <gl/glew.h>


// TODO: Add flags to control STATIC/DYNAMIC DRAW
struct ElementBufferRange
{
	GLsizei offset;
	GLsizei size;
};

template <typename type>
class VertexArray
{
public:
	VertexArray(int maxVertexCount, int maxElementCount) : m_capacityVerts(maxVertexCount), m_capacityElts(maxElementCount)
	{
		m_sizeElts = 0;
		m_sizeVerts = 0;

		// Vertex Arrray Object
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		// Vertex Buffer

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		glBufferData(GL_ARRAY_BUFFER, m_capacityVerts * sizeof(type), nullptr, GL_DYNAMIC_DRAW);

		// Vertex Buffer Attributes:
		// Vertex Positions

		if constexpr (std::is_same<type, Vertex3D>())
		{
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, pos));
			glEnableVertexAttribArray(0);
			// Normals
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, normal));
			glEnableVertexAttribArray(1);
			// Texture Coords
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, texCoord));
			glEnableVertexAttribArray(2);
		}
		else if constexpr (std::is_same<type, Vertex2D>())
		{
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, pos));
			glEnableVertexAttribArray(0);
			// Texture Coords
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, texCoord));
			glEnableVertexAttribArray(1);
		}
		else
			static_assert(true, "Please use one of the supported Vertex types!");

		// Element Buffer		
		glGenBuffers(1, &m_ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxElementCount * 3 * sizeof(unsigned int), nullptr, GL_STATIC_DRAW);

		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}

	~VertexArray()
	{
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_ebo);
		glDeleteVertexArrays(1, &m_vao);
	}

	ElementBufferRange PushVertices(const std::vector<type>& verts, std::vector<unsigned int> indices)
	{
		// Add Vertices at the End, resizing as neccessary

		GLsizei newVertexCount = m_sizeVerts + verts.size();
		GLsizei newElementCount = m_sizeElts + indices.size();

		assert(indices.size() % 3 == 0 && "Number of indcies must be divisible by 3, because triangle elements are used");

		for (auto& ind : indices)
			ind += m_sizeVerts;

		assert(newVertexCount < m_capacityVerts && newElementCount < m_capacityElts);

		WriteRangeVerts(m_sizeVerts, verts.size(), verts.data());
		WriteRangeElts(m_sizeElts, indices.size(), indices.data());
		//glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		//glBufferSubData(GL_ARRAY_BUFFER, m_sizeVerts * sizeof(type), verts.size() * sizeof(type), verts.data());
		////glBindBuffer(GL_ARRAY_BUFFER, 0);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		//glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_sizeElts * sizeof(unsigned int), indices.size() * sizeof(unsigned int), indices.data());
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		
		ElementBufferRange EBRange{ m_sizeElts, indices.size() };
		m_sizeElts = newElementCount;
		m_sizeVerts = newVertexCount;
		return EBRange;
	}

	void Bind()
	{
		glBindVertexArray(m_vao);
	}
	void Unbind()
	{
		glBindVertexArray(0);
	}

protected:

	void WriteRangeVerts(int start, size_t count, const type* data)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferSubData(GL_ARRAY_BUFFER, start * sizeof(type), count * sizeof(type), data);
	}
	void WriteRangeElts(int start, size_t count, const unsigned int* data)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, start * sizeof(unsigned int), count * sizeof(unsigned int), data);
	}

	GLsizei m_capacityVerts;
	GLsizei m_capacityElts;

	GLsizei m_sizeVerts;
	GLsizei m_sizeElts;

	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ebo;
};


template <typename type>
class VertexArrayRing : public VertexArray<type>
{
public:
	VertexArrayRing(int maxVertexCount, int maxElementCount) : VertexArray<type>(maxVertexCount, maxElementCount) {};
	
	ElementBufferRange PushVertices(const std::vector<type>& verts, std::vector<unsigned int> indices)
	{
		assert(indices.size() % 3 == 0 && "Number of indcies must be divisible by 3, because triangle elements are used");

		GLsizei newVertexCount = m_sizeVerts + static_cast<GLsizei>(verts.size());
		GLsizei newElementCount = m_sizeElts + static_cast<GLsizei>(indices.size());

		bool isVertexArrayFull = newVertexCount > m_capacityVerts;
		bool isElementArrayFull = newElementCount > m_capacityElts;

		if (isVertexArrayFull || isElementArrayFull)
		{	// reset write locaton to start
			m_sizeVerts = 0;
			m_sizeElts = 0;

			newVertexCount = static_cast<GLsizei>(verts.size());
			newElementCount = static_cast<GLsizei>(indices.size());
		}

		for (auto& ind : indices)
			ind += m_sizeVerts;

		WriteRangeVerts(m_sizeVerts, verts.size(), verts.data());
		WriteRangeElts(m_sizeElts, indices.size(), indices.data());
		//glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		//glBufferSubData(GL_ARRAY_BUFFER, m_sizeVerts * sizeof(type), verts.size() * sizeof(type), verts.data());
		////glBindBuffer(GL_ARRAY_BUFFER, 0);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		//glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_sizeElts * sizeof(unsigned int), indices.size() * sizeof(unsigned int), indices.data());
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		ElementBufferRange EBRange{ m_sizeElts, (GLsizei)indices.size() };
		m_sizeElts = newElementCount;
		m_sizeVerts = newVertexCount;
		return EBRange;
	}
};