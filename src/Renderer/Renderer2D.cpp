#include "Renderer2D.h"

#include "Utils/math.h"

// Idea: Assemble the vertex array each frame and then draw...
// drawables:
// texture Atlas ID
// texture Coords
// verts
// transform2D

// Components
// isDrawable
// isStatic
// SpriteInfo -> texture Coords, Atlas ID
// Verts -> QUAD component
// transform

// Renderer's job:
// Assemble Vertex Array(s) on CPU
// keep track of uniforms (transforms, Atlas ID)
// keep track of Element buffer offsets


// TODO: expose the size
FrameVertexBuffer::FrameVertexBuffer()
{
	m_VertexArray = std::make_unique<VertexArray<Vertex2D>>(4000, 6000);
}

void FrameVertexBuffer::Submit()
{
	
}

void FrameVertexBuffer::AddDrawable(eID entity)
{
	const auto& [vertices, indices] = MakeQuadVerts(entity);
	const auto & indexRange = m_VertexArray->PushVertices(vertices, indices);

	m_drawinfo.push_back({ entity, indexRange });
}

std::tuple<std::vector<Vertex2D>, std::vector<unsigned int>> FrameVertexBuffer::MakeQuadVerts(eID entity)
{
	
	const auto& quad = manager.GetComponent<Quad>(entity);
	const auto& texCoords = manager.GetComponent<TexCoords<4>>(entity);

	auto vertices = std::vector<Vertex2D>();
	vertices.reserve(4);

	auto indices = std::vector<unsigned int>();

	for (int i = 0; i < 4; i++)
	{
		Vertex2D vert;
		
		const glm::vec2 texCoord = texCoords.coords[i].to_glm();
		const glm::vec2 pos = quad.vertexData.vertices[i].to_glm();

		vert.pos = pos;
		vert.texCoord = texCoord;
		vertices.push_back(vert);
	}

	// assumes quad verts are defined counter clock wise
	indices = {0, 1, 2, 0, 2, 3};

	return std::make_tuple(vertices, indices);
}

Renderer2D::Renderer2D()
{
	m_shader = std::make_unique<SpriteShader>(SOURCE_DIRECTORY + std::string("/resource/shaders/test"));
}

void Renderer2D::Draw(FrameVertexBuffer& vertexBuffer, const glm::mat3& cameraProjection)
{
	
	vertexBuffer.Bind();

	for (auto& drawInfo : vertexBuffer.GetDrawInfo())
	{
		const auto& tForm = manager.GetComponent<Transform2D>(drawInfo.entity);
		
		auto& transform = makeRotation(tForm.rotation);


		transform[2][0] = tForm.pos.x;
		transform[2][1] = tForm.pos.y;

		m_shader->Bind();
		
		m_shader->model.Set(transform);

		m_shader->projection.Set(cameraProjection);
		std::array color = { 1.0f, 1.0f, 1.0f };
		m_shader->spriteColor.Set(color);

		const auto& range = drawInfo.indexRange;

		glDrawElements(GL_TRIANGLES, range.size, GL_UNSIGNED_INT, (GLvoid*)(range.offset * sizeof(unsigned int)));
	}

	vertexBuffer.Unbind();
}

