#pragma once

#include <string>
#include "Portals/src/controls/observer.h"
#include "Portals/src/objects/body.h"

#include "Portals/src/renderer/shader.h"
#include "Portals/src/renderer/skybox.h"
#include "Portals/src/renderer/textured_shaded_mesh.h"

std::string ParseShader(const std::string& filepath);


// Function signature of the force function. There will be a version of the Update() method, that takes in a customizable force function
// to calculate the acceleration of the bodies.
// typedef std::vector<Vec3D>(*AccelerationFunction)(const std::vector<Body>& bodies);
typedef void(*AccelerationFunction)(std::vector<Body>& bodies, std::vector<Vec3D>& accelerations);

class Scene
{
public:
//	Scene() = default;
	Scene(const std::string& filename);
	~Scene();

	void Update(float deltaTime);
	void Update(float deltaTime, AccelerationFunction accelerationFunc);
	void Draw(Observer obs);
	void SetAspectRatio(float aspectRatio);

private:
	std::vector<Body> m_Bodies;
	std::vector<Vec3D> m_Accelerations; // I just created this vector here, so the program doesnt need to allocate memory each time it calculates accelerations
	std::vector<int> m_MeshIndices; // this and m_BodyTypes are kind of redundant, since they essentially code the same info, just in different ways

//	Shader m_ColourShader; // (ParseShader("src/renderer/shader_sources/vertex_shader.glsl"), ParseShader("src/renderer/shader_sources/fragment_shader.glsl"));
	Shader m_TextureShader; // (ParseShader("src/renderer/shader_sources/vertex_shader_textured.glsl"), ParseShader("src/renderer/shader_sources/fragment_shader_textured.glsl"));

	Skybox m_Skybox;

	std::vector<TexturedShadedMesh> m_Meshes;
};







