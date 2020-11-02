#pragma once

#include "Portals/src/renderer/buffer.h"
#include "Portals/src/renderer/shader.h"
#include "Portals/src/controls/observer.h"


class portal
{
public:
	portal();
	~portal();

	void Draw();


private:

	uint32_t m_VertexArray;
	OpenGLVertexBuffer m_VertexBuffer; // layout should be: 3 floats - vertex position / 2 floats - texture coordinates
	OpenGLIndexBuffer m_IndexBuffer;
	uint32_t m_Texture;
	uint32_t m_Framebuffer;


};


