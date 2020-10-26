
#define STB_IMAGE_IMPLEMENTATION

#include "skybox.h"

#include "glad/glad.h"

#include "vendor/stb_image/stb_image.h"

// #include "glad/glad.h"

Skybox::Skybox()
{
	std::vector<std::string> texture_files = {
		"assets/back.png",
		"assets/front.png",
		"assets/left.png",
		"assets/right.png",
		"assets/up.png",
		"assets/down.png"
	};

	Init(texture_files, 2);
}

Skybox::Skybox(std::vector<std::string> texture_files, int planeResolution)
{
	Init(texture_files, planeResolution);
}

Skybox::~Skybox()
{
	glDeleteVertexArrays(1, &m_VertexArray);
	m_VertexBuffer.~OpenGLVertexBuffer();
	m_IndexBuffer.~OpenGLIndexBuffer();
//	m_TextureShader.~Shader();
}

void Skybox::Draw(Observer obs)
{
	glDisable(GL_DEPTH_TEST);

	m_TextureShader.Bind();
	glBindVertexArray(m_VertexArray);
//	m_VertexBuffer.Bind();
//	m_IndexBuffer.Bind();

	m_TextureShader.UploadUniformMat3("observer_orientation", obs.orientation.Glm());
	m_TextureShader.UploadUniformFloat("zoom_level", obs.zoom_level);

	glDrawElements(GL_TRIANGLES, m_IndexBuffer.m_Count, GL_UNSIGNED_INT, nullptr);

	glEnable(GL_DEPTH_TEST);
}


void Skybox::Init(std::vector<std::string> texture_files, int planeResolution)
{
	glCreateVertexArrays(1, &m_VertexArray);
	glBindVertexArray(m_VertexArray);


//	Create Vertex Data with texture coordinates and face indices
	Shape3D face_U = CreatePlane(planeResolution, planeResolution);
	face_U.center();
	face_U.normalize();

	Shape3D face_D = face_U;
	Shape3D face_F = face_U;
	Shape3D face_B = face_U;
	Shape3D face_L = face_U;
	Shape3D face_R = face_U;
	std::vector<float> allVertexDataWithTextureCoords;
	allVertexDataWithTextureCoords.resize(6 * 6 * face_U.vertices.size());

	{
		face_L.rotate(Rotation(3.1415926535f / 2.0f, 3.1415926535f / 2.0f, 3.1415926535f / 2.0f));
		face_R.rotate(Rotation(3.1415926535f / 2.0f, 3.1415926535f / 2.0f, 3.1415926535f / 2.0f));
		face_F.rotate(Rotation(3.1415926535f / 2.0f, 3.1415926535f / 2.0f, 0.0f));
		face_B.rotate(Rotation(3.1415926535f / 2.0f, 3.1415926535f / 2.0f, 0.0f));

		face_U.center(); face_U.normalize();
		face_D.center(); face_D.normalize();
		face_L.center(); face_L.normalize();
		face_R.center(); face_R.normalize();
		face_F.center(); face_F.normalize();
		face_B.center(); face_B.normalize();

		// 1/sqrt(2) = 0.7071067811865475
		float d = 0.707106781187f;
		face_F.translate({ 0.0f, -d, 0.0f });
		face_B.translate({ 0.0f,  d,  0.0f });
		face_L.translate({ -d, 0.0f, 0.0f });
		face_R.translate({ d, 0.0f, 0.0f });
		face_U.translate({ 0.0f, 0.0f,  d });
		face_D.translate({ 0.0f, 0.0f, -d });

		int j = 0;
		for (int i = 0; i < face_U.vertices.size(); i++)
		{
			allVertexDataWithTextureCoords[6 * j + 0] = face_U.vertices[0].x; // vertex positions
			allVertexDataWithTextureCoords[6 * j + 1] = face_U.vertices[0].y;
			allVertexDataWithTextureCoords[6 * j + 2] = face_U.vertices[0].z;
			allVertexDataWithTextureCoords[6 * j + 3] = (float)(i % (planeResolution +1)/(float)planeResolution); // texture coords
			allVertexDataWithTextureCoords[6 * j + 4] = (float)(i / (planeResolution +1)/(float)planeResolution);
			allVertexDataWithTextureCoords[6 * j + 5] = 0.0f; // texture ID
			j++;
		}
		for (int i = 0; i < face_D.vertices.size(); i++)
		{
			allVertexDataWithTextureCoords[6 * j + 0] = face_D.vertices[0].x;
			allVertexDataWithTextureCoords[6 * j + 1] = face_D.vertices[0].y;
			allVertexDataWithTextureCoords[6 * j + 2] = face_D.vertices[0].z;
			allVertexDataWithTextureCoords[6 * j + 3] = (float)(i % (planeResolution + 1) / (float)planeResolution); // texture coords
			allVertexDataWithTextureCoords[6 * j + 4] = (float)(i / (planeResolution + 1) / (float)planeResolution);
			allVertexDataWithTextureCoords[6 * j + 5] = 1.0f;
			j++;
		}
		for (int i = 0; i < face_F.vertices.size(); i++)
		{
			allVertexDataWithTextureCoords[6 * j + 0] = face_F.vertices[0].x;
			allVertexDataWithTextureCoords[6 * j + 1] = face_F.vertices[0].y;
			allVertexDataWithTextureCoords[6 * j + 2] = face_F.vertices[0].z;
			allVertexDataWithTextureCoords[6 * j + 3] = (float)(i % (planeResolution + 1) / (float)planeResolution); // texture coords
			allVertexDataWithTextureCoords[6 * j + 4] = (float)(i / (planeResolution + 1) / (float)planeResolution);
			allVertexDataWithTextureCoords[6 * j + 5] = 2.0f;
			j++;
		}
		for (int i = 0; i < face_B.vertices.size(); i++)
		{
			allVertexDataWithTextureCoords[6 * j + 0] = face_B.vertices[0].x;
			allVertexDataWithTextureCoords[6 * j + 1] = face_B.vertices[0].y;
			allVertexDataWithTextureCoords[6 * j + 2] = face_B.vertices[0].z;
			allVertexDataWithTextureCoords[6 * j + 3] = (float)(i % (planeResolution + 1) / (float)planeResolution); // texture coords
			allVertexDataWithTextureCoords[6 * j + 4] = (float)(i / (planeResolution + 1) / (float)planeResolution);
			allVertexDataWithTextureCoords[6 * j + 5] = 3.0f;
			j++;
		}
		for (int i = 0; i < face_L.vertices.size(); i++)
		{
			allVertexDataWithTextureCoords[6 * j + 0] = face_L.vertices[0].x;
			allVertexDataWithTextureCoords[6 * j + 1] = face_L.vertices[0].y;
			allVertexDataWithTextureCoords[6 * j + 2] = face_L.vertices[0].z;
			allVertexDataWithTextureCoords[6 * j + 3] = (float)(i % (planeResolution + 1) / (float)planeResolution); // texture coords
			allVertexDataWithTextureCoords[6 * j + 4] = (float)(i / (planeResolution + 1) / (float)planeResolution);
			allVertexDataWithTextureCoords[6 * j + 5] = 4.0f;
			j++;
		}
		for (int i = 0; i < face_R.vertices.size(); i++)
		{
			allVertexDataWithTextureCoords[6 * j + 0] = face_R.vertices[0].x;
			allVertexDataWithTextureCoords[6 * j + 1] = face_R.vertices[0].y;
			allVertexDataWithTextureCoords[6 * j + 2] = face_R.vertices[0].z;
			allVertexDataWithTextureCoords[6 * j + 3] = (float)(i % (planeResolution + 1) / (float)planeResolution); // texture coords
			allVertexDataWithTextureCoords[6 * j + 4] = (float)(i / (planeResolution + 1) / (float)planeResolution);
			allVertexDataWithTextureCoords[6 * j + 5] = 5.0f;
			j++;
		}

	}

	// do this after the vertex data has been initialized
	m_VertexBuffer = std::move(OpenGLVertexBuffer((float*)&allVertexDataWithTextureCoords[0], allVertexDataWithTextureCoords.size() * sizeof(float)));
	m_VertexBuffer.SetLayout({
		{ShaderDataType::Float3, "aPos"},
		{ShaderDataType::Float2, "aTexCoord"},
		{ShaderDataType::Float, "aTexID"}
	});

//	Create Index Data with texture coordinates
	std::vector<uint32_t> allIndexData;
	int max_index = 0;
	
	allIndexData.resize(6 * face_U.indices.size());
	{
		int j = 0;
		int max_index_start = max_index;
		for (int i = 0; i < face_U.indices.size(); i++)
		{
			allIndexData[j] = face_U.indices[i] + max_index_start;
			max_index = (face_U.indices[i] + max_index_start) > max_index ? (face_U.indices[i] + max_index_start) : max_index;
			j++;
		}

		max_index_start = max_index+1;
		for (int i = 0; i < face_D.indices.size(); i++)
		{
			allIndexData[j] = face_D.indices[i] + max_index_start;
			max_index = (face_D.indices[i] + max_index_start) > max_index ? (face_D.indices[i] + max_index_start) : max_index;
			j++;
		}

		max_index_start = max_index + 1;
		for (int i = 0; i < face_F.indices.size(); i++)
		{
			allIndexData[j] = face_F.indices[i] + max_index_start;
			max_index = (face_F.indices[i] + max_index_start) > max_index ? (face_F.indices[i] + max_index_start) : max_index;
			j++;
		}

		max_index_start = max_index + 1;
		for (int i = 0; i < face_B.indices.size(); i++)
		{
			allIndexData[j] = face_B.indices[i] + max_index_start;
			max_index = (face_B.indices[i] + max_index_start) > max_index ? (face_B.indices[i] + max_index_start) : max_index;
			j++;
		}

		max_index_start = max_index + 1;
		for (int i = 0; i < face_L.indices.size(); i++)
		{
			allIndexData[j] = face_L.indices[i] + max_index_start;
			max_index = (face_L.indices[i] + max_index_start) > max_index ? (face_L.indices[i] + max_index_start) : max_index;
			j++;
		}

		max_index_start = max_index + 1;
		for (int i = 0; i < face_R.indices.size(); i++)
		{
			allIndexData[j] = face_R.indices[i] + max_index_start;
			max_index = (face_R.indices[i] + max_index_start) > max_index ? (face_R.indices[i] + max_index_start) : max_index;
			j++;
		}
	}

	m_IndexBuffer = OpenGLIndexBuffer((uint32_t*)&allIndexData[0], allIndexData.size());

//	Load images and create textures out of them
	std::vector<uint32_t> rendererID; rendererID.resize(texture_files.size());
	for (uint32_t i = 0; i < texture_files.size(); i++)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			data = stbi_load(texture_files[i].c_str(), &width, &height, &channels, 0);
		}
		if (!data) { std::cout << "Failed to load image: " << texture_files[i]; }

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &rendererID[i]);
		glTextureStorage2D(rendererID[i], 1, internalFormat, width, height);

		glTextureParameteri(rendererID[i], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(rendererID[i], GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(rendererID[i], GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(rendererID[i], GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(rendererID[i], 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);

		glBindTextureUnit(i, rendererID[i]);

		stbi_image_free(data);
	}

	// Create the shader that will draw the skybox
	m_TextureShader = Shader(ParseShader("src/renderer/shader_sources/vertex_shader_skybox.glsl"), ParseShader("src/renderer/shader_sources/fragment_shader_skybox.glsl"));

	m_TextureShader.SearchAndAddUniform("observer_orientation");
	m_TextureShader.SearchAndAddUniform("zoom_level");
	m_TextureShader.SearchAndAddUniform("u_Textures"); // there is a uniform in the fragment shader as well!

	int samplers[32];
	for (uint32_t i = 0; i < 32; i++) { samplers[i] = i; }
	m_TextureShader.UploadUniformIntArray("u_Textures", samplers, 32);

	// Unbind
	glBindVertexArray(0);
}




// int samplers[s_Data.MaxTextureSlots];
// for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
// {
// 	samplers[i] = i;
// }
// 
// s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
// s_Data.TextureShader->Bind();
// //		s_Data.TextureShader->SetInt("u_Texture", 0);
// s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

