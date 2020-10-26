
#include "scene.h"

#include "glad/glad.h"

Scene::Scene(OpenGLVertexBuffer& vb, OpenGLIndexBuffer& ib)
{
//	glGenVertexArrays(1, &m_VertexArray);
	m_VertexBuffer = std::move(vb);
	m_IndexBuffer = ib;
}

Scene::~Scene()
{
	m_VertexBuffer.~OpenGLVertexBuffer();
	m_IndexBuffer.~OpenGLIndexBuffer();
}

void Scene::SetObserver(Shader& shader)
{
	shader.Bind();
	shader.UploadUniformFloat3("observer_translation", m_Observer.translation.Glm());
	shader.UploadUniformMat3("observer_orientation", m_Observer.orientation.Glm());
	shader.UploadUniformFloat("zoom_level", m_Observer.zoom_level);
}

void Scene::Draw()
{
//	glBindVertexArray(m_VertexArray);
	m_VertexBuffer.Bind();
	m_IndexBuffer.Bind();
	glDrawElements(GL_TRIANGLES, m_IndexBuffer.m_Count, GL_UNSIGNED_INT, nullptr);
}
