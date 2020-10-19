
#include "scene.h"

#include "glad/glad.h"

Scene::Scene(OpenGLVertexBuffer& vb, OpenGLIndexBuffer& ib)
{
//	glGenVertexArrays(1, &m_VertexArray);
	m_VertexBuffer = vb;
	m_IndexBuffer = ib;
}

Scene::~Scene()
{
	m_VertexBuffer.~OpenGLVertexBuffer();
	m_IndexBuffer.~OpenGLIndexBuffer();
}

void Scene::Draw()
{
//	glBindVertexArray(m_VertexArray);
	m_VertexBuffer.Bind();
	m_IndexBuffer.Bind();
	glDrawElements(GL_TRIANGLES, m_IndexBuffer.m_Count, GL_UNSIGNED_INT, nullptr);
}
