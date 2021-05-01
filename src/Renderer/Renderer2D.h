#pragma once

#include "VertexArray.h"
#include "ECS/ecs.h"

#include "SpriteShader.h"

#include <memory>

extern ECSManager manager;

class FrameVertexBuffer;

class Renderer2D
{
public:
	Renderer2D();
	void Draw(FrameVertexBuffer& vertexBuffer, const glm::mat3& cameraProjection);

private:
	
	std::unique_ptr<SpriteShader> m_shader;
};

struct IndexedDrawInfo
{
	eID entity;
	ElementBufferRange indexRange;
};

class FrameVertexBuffer
{
public:
	FrameVertexBuffer();
	void Submit();

	void Bind() { m_VertexArray->Bind(); }
	void Unbind() { m_VertexArray->Unbind(); };
	const std::vector<IndexedDrawInfo>& GetDrawInfo() const { return m_drawinfo; }

	void AddDrawable(eID entity);
private:

	//void IndexedDraw()
	std::unique_ptr<VertexArrayRing<Vertex2D>> m_VertexArray;

	
	std::vector<IndexedDrawInfo> m_drawinfo;

	std::tuple<std::vector<Vertex2D>, std::vector<unsigned int>> MakeQuadVerts(eID entity);
};

