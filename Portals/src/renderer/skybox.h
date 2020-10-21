#pragma once

#include <vector>

#include "Portals/src/utils/shape_creator.h"
#include "buffer.h"
#include "shader.h"
#include "Portals/src/controls/observer.h"

class Skybox
{
public:
	Skybox();
	Skybox(std::vector<std::string> texture_files, int planeResolution);
	~Skybox();


	void Draw(Observer obs);


private:
	void Init(std::vector<std::string> texture_files, int planeResolution);

private:
	uint32_t m_VertexArray;
	OpenGLVertexBuffer m_VertexBuffer;
	OpenGLIndexBuffer m_IndexBuffer;
	Shader m_TextureShader;
};






