
#include "mesh.h"
#include <fstream>




std::string BodyTypeToFilepath(BodyType type)
{
	switch (type)
	{
	case BodyType::cube_low_res:
		return "assets/textured_meshes/cube_1.txt";
	case BodyType::cube:
		return "assets/textured_meshes/cube_2.txt";
	case BodyType::cube_high_res:
		return "assets/textured_meshes/cube_8.txt";
	case BodyType::sphere_low_res:
		return "assets/textured_meshes/sphere_2.txt";
	case BodyType::sphere:
		return "assets/textured_meshes/sphere_4.txt";
	case BodyType::sphere_high_res:
		return "assets/textured_meshes/sphere_10.txt";
	default:
		return "assets/textured_meshes/cube_1.txt";
	}
}



TexturedMesh::TexturedMesh()
{
	Init(BodyType::cube_low_res);
}

TexturedMesh::TexturedMesh(BodyType type)
{
	Init(type);
}

void TexturedMesh::Init(BodyType type)
{
	std::ifstream myfile(BodyTypeToFilepath(type).c_str());
	int v_count = 0, i_count = 0;

	myfile >> v_count;
	myfile >> i_count;

	vertexData.resize(v_count*5); // the layout is a vec3 for positions and vec2 for texture coords, that's why the hardcoded 5
	indices.resize(i_count);

	for (int i = 0; i < v_count; i++)
	{
		myfile >> vertexData[5*i+0];
		myfile >> vertexData[5*i+1];
		myfile >> vertexData[5*i+2];
		myfile >> vertexData[5*i+3];
		myfile >> vertexData[5*i+4];
	}

	for (int i = 0; i < i_count; i++)
		myfile >> indices[i];

	myfile.close();

}




