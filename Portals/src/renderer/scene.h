#pragma once

#include "buffer.h"

class Scene
{
public:

	Scene() = default;
	Scene(OpenGLVertexBuffer& vb, OpenGLIndexBuffer& ib);
	~Scene();

	void Draw();


private:
	uint32_t m_VertexArray;

	std::vector<float> m_VertexData;
	OpenGLVertexBuffer m_VertexBuffer;
	std::vector<int> m_IndexData;
	OpenGLIndexBuffer m_IndexBuffer;

};





