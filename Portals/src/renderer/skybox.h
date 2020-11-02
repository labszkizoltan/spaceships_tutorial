#pragma once

#include <vector>

#include "Portals/src/utils/shape_creator.h"
#include "buffer.h"
#include "shader.h"
#include "Portals/src/controls/observer.h"


/*
class Skybox
{
public:
	Skybox();
//	Skybox(std::vector<std::string> texture_files);

//	Skybox(const Skybox& other); // copy constructor
//	Skybox& operator=(const Skybox& other); // copy assignment
//	Skybox(Skybox&& other) noexcept; // move constructor
//	Skybox& operator=(Skybox&& other) noexcept; // move assignment
	~Skybox();

	void Draw(Observer obs);

private:
//	void Init(std::vector<std::string> texture_files);

private:
	uint32_t m_VertexArray, m_VertexBuffer, m_IndexBuffer, m_Shader, m_Texture;
};
*/




// First iteration of this class, above is a reworked version

class Skybox
{
public:
	Skybox();
	Skybox(std::vector<std::string> texture_files, int planeResolution);

	Skybox(const Skybox& other) = delete; // copy constructor
	Skybox& operator=(const Skybox& other) = delete; // copy assignment
	Skybox(Skybox&& other) noexcept; // move constructor
	Skybox& operator=(Skybox&& other) noexcept; // move assignment
	~Skybox();

	void Draw(Observer obs);
	void SetShaderAspectRatio(float aspectRatio);

private:
	void Init(std::vector<std::string> texture_files, int planeResolution);

private:
	uint32_t m_VertexArray;
	OpenGLVertexBuffer m_VertexBuffer;
	OpenGLIndexBuffer m_IndexBuffer;
	Shader m_TextureShader;
	std::vector<uint32_t> m_Textures;
};



