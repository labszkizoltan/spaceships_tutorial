#pragma once

#include "Portals/src/renderer/buffer.h"
#include "Portals/src/utils/Vector_2D.h"
#include "Portals/src/utils/Vector_3D.h"

#include "Portals/src/renderer/shader.h"


class TexturedShadedMesh
{
public:
	TexturedShadedMesh() = default;
	TexturedShadedMesh(const std::string& vertexFilepath, const std::string& textureFilepath);

	TexturedShadedMesh(const TexturedShadedMesh& other) = delete; // copy constructor
	TexturedShadedMesh& operator=(const TexturedShadedMesh& other) = delete; // copy assignment
	TexturedShadedMesh(TexturedShadedMesh&& other) noexcept; // move constructor
	TexturedShadedMesh& operator=(TexturedShadedMesh&& other) noexcept; // move assignment

	~TexturedShadedMesh();

	void Draw();

private:
	//	void Init(BodyType type);

private:
	uint32_t m_VertexArray;
	OpenGLVertexBuffer m_VertexBuffer; // layout should be: 3 floats - vertex position / 3 floats - surface normal / 2 floats - texture coordinates
	OpenGLIndexBuffer m_IndexBuffer;
	uint32_t m_Texture;
};


