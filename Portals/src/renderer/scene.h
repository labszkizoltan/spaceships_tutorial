#pragma once

#include "buffer.h"
#include "shader.h"
#include "Portals/src/controls/observer.h"

class Scene
{
public:

	Scene() = default;
	Scene(OpenGLVertexBuffer& vb, OpenGLIndexBuffer& ib);
	~Scene();

	void SetObserver(Shader& shader);
	void Draw();

public:
	Observer m_Observer;

private:
	uint32_t m_VertexArray;

	std::vector<float> m_VertexData;
	OpenGLVertexBuffer m_VertexBuffer;
	std::vector<int> m_IndexData;
	OpenGLIndexBuffer m_IndexBuffer;

};





