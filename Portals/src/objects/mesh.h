#pragma once

#include "Portals/src/renderer/buffer.h"
#include "Portals/src/utils/Vector_2D.h"
#include "Portals/src/utils/Vector_3D.h"

enum class BodyType
{
	cube_low_res, cube, cube_high_res,
	sphere_low_res, sphere, sphere_high_res
};

class TexturedMesh
{
public:
	TexturedMesh();
	TexturedMesh(BodyType type);
	~TexturedMesh() = default;

private:
	void Init(BodyType type);

private:
//	std::vector<Vec3D> vertexPositions;
//	std::vector<Vec2D> textureCoordinates;
	std::vector<float> vertexData; // layout should be 
	std::vector<uint32_t> indices;

	OpenGLVertexBuffer m_Vertices; // these classes have a default construtor
	OpenGLIndexBuffer m_Indices;

};





