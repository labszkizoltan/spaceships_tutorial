#pragma once

#include "buffer.h"
#include "shader.h"
#include "Portals/src/controls/observer.h"
#include "Portals/src/utils/Vector_3D.h"

// This class is rather a coloured mesh, not a scene

class ColouredMesh
{
public:

	ColouredMesh() = default;
	ColouredMesh(OpenGLVertexBuffer& vb, OpenGLIndexBuffer& ib);
	ColouredMesh(const std::vector<Vec3D>& vertexAndColorData, const std::vector<uint32_t>& indexData);
	ColouredMesh(const std::string& filename);
	~ColouredMesh();

	void Draw();

private:
	uint32_t m_VertexArray;
	OpenGLVertexBuffer m_VertexBuffer;
	OpenGLIndexBuffer m_IndexBuffer;

};




