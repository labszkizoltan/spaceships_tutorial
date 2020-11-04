#pragma once

#include <string>
#include "Portals/src/controls/observer.h"
#include "Portals/src/objects/body.h"

#include "Portals/src/renderer/shader.h"
#include "Portals/src/renderer/skybox.h"

std::string ParseShader(const std::string& filepath);

class Scene
{
public:
//	Scene() = default;
	Scene(const std::string& filename);
	~Scene();

	void Update(float deltaTime);
	void Draw(Observer obs);


private:
	std::vector<Body> m_Bodies;

//	Shader m_ColourShader; // (ParseShader("src/renderer/shader_sources/vertex_shader.glsl"), ParseShader("src/renderer/shader_sources/fragment_shader.glsl"));
	Shader m_TextureShader; // (ParseShader("src/renderer/shader_sources/vertex_shader_textured.glsl"), ParseShader("src/renderer/shader_sources/fragment_shader_textured.glsl"));

	Skybox m_Skybox;

	std::vector<TexturedMesh> m_Meshes;

};







