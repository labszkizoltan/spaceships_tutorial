
#define STB_IMAGE_IMPLEMENTATION

#include "skybox.h"

#include "glad/glad.h"

#include "vendor/stb_image/stb_image.h"

#include "renderer.h"

Skybox::Skybox()
	: m_VertexArray(0), m_VertexBuffer(), m_IndexBuffer(), m_TextureShader()
{
	std::vector<std::string> texture_files = {
		"assets/up.png",
		"assets/down.png",
		"assets/front.png",
		"assets/back.png",
		"assets/left.png",
		"assets/right.png"
	};

	Init(texture_files, 10);
}

Skybox::Skybox(std::vector<std::string> texture_files, int planeResolution)
{
	Init(texture_files, planeResolution);
}

// // copy constructor
// Skybox::Skybox(const Skybox & other)
// 	: m_VertexArray(other.m_VertexArray), m_VertexBuffer(other.m_VertexBuffer), m_IndexBuffer(other.m_IndexBuffer), m_TextureShader(other.m_TextureShader)
// {
// 	std::cout << "Skybox::Skybox(const Skybox & other) - copy constructor was called. \n";
// }
//
//
// // copy assignment
// Skybox & Skybox::operator=(const Skybox & other)
// {
// 	// perhaps it should be checked if " this != &other " just like in the move assignment operator
//
// 	glDeleteVertexArrays(1, &m_VertexArray);
// 	m_VertexBuffer.~OpenGLVertexBuffer();
// 	m_IndexBuffer.~OpenGLIndexBuffer();
// 	m_TextureShader.~Shader();
// 	return *this = Skybox(other);
// }

// move constructor
Skybox::Skybox(Skybox && other) noexcept
{
	m_VertexArray = other.m_VertexArray;
	m_VertexBuffer = std::move(other.m_VertexBuffer);
	m_IndexBuffer = std::move(other.m_IndexBuffer);
	m_TextureShader = std::move(other.m_TextureShader);
	m_Textures = std::move(other.m_Textures);

	other.m_VertexArray = 0;
	other.m_VertexBuffer.m_RendererID = 0;
	other.m_IndexBuffer.m_RendererID = 0;
	other.m_TextureShader.m_RendererID = 0;
	for (int i = 0; i < other.m_Textures.size(); i++)
		other.m_Textures[i] = 0;
}

// move assignment
Skybox & Skybox::operator=(Skybox && other) noexcept
{
	if (this != &other)
	{
		glDeleteVertexArrays(1, &m_VertexArray);
		m_VertexBuffer.~OpenGLVertexBuffer();
		m_IndexBuffer.~OpenGLIndexBuffer();
		m_TextureShader.~Shader();
		glDeleteTextures(m_Textures.size(), &m_Textures[0]);

		m_VertexArray = other.m_VertexArray;
		m_VertexBuffer = std::move(other.m_VertexBuffer);
		m_IndexBuffer = std::move(other.m_IndexBuffer);
		m_TextureShader = std::move(other.m_TextureShader);
		m_Textures = std::move(other.m_Textures);

		other.m_VertexArray = 0;
		other.m_VertexBuffer.m_RendererID = 0;
		other.m_IndexBuffer.m_RendererID = 0;
		other.m_TextureShader.m_RendererID = 0;
		for (int i = 0; i < other.m_Textures.size(); i++)
			other.m_Textures[i] = 0;
	}

	return *this;
}


Skybox::~Skybox()
{
	glDeleteVertexArrays(1, &m_VertexArray);
	m_VertexBuffer.~OpenGLVertexBuffer();
	m_IndexBuffer.~OpenGLIndexBuffer();
	m_TextureShader.~Shader();

	for (int i = 0; i < m_Textures.size(); i++)
	{
		glDeleteTextures(1, &m_Textures[i]);
	}
}

void Skybox::Draw(Observer obs)
{
	glBindVertexArray(m_VertexArray);
	m_VertexBuffer.Bind();
	m_IndexBuffer.Bind();
	m_TextureShader.Bind();

	for(int i=0; i<m_Textures.size(); i++)
		glBindTextureUnit(i, m_Textures[i]);

	//for (auto ID : m_Textures)
	for(int i=0; i< m_Textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTextureUnit(i, m_Textures[i]);
	}

//	m_TextureShader.UploadUniformMat3("observer_orientation", obs.orientation.Glm());
//	m_TextureShader.UploadUniformFloat("zoom_level", obs.zoom_level);
	obs.SetObserverInShader(m_TextureShader);

	glDrawElements(GL_TRIANGLES, m_IndexBuffer.m_Count, GL_UNSIGNED_INT, nullptr);

	glClear(GL_DEPTH_BUFFER_BIT);
}

void Skybox::SetShaderAspectRatio(float aspectRatio)
{
	m_TextureShader.Bind();
	m_TextureShader.UploadUniformFloat("aspect_ratio", aspectRatio);
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
		face_R.rotate(Rotation(3.1415926535f / 2.0f, {0,1,0}));
		face_L.rotate(Rotation(3.1415926535f / 2.0f, {0,-1,0}));
		face_F.rotate(Rotation(3.1415926535f / 2.0f, {-1,0,0}));
		face_B.rotate(Rotation(3.1415926535f / 2.0f, {1,0,0}));
		face_U.rotate(Rotation(3.1415926535f, {1,0,0}));

		face_U.center(); face_U.normalize(); face_U.scale(1.05);
		face_D.center(); face_D.normalize(); face_D.scale(1.05);
		face_R.center(); face_L.normalize(); face_R.scale(1.05);
		face_L.center(); face_R.normalize(); face_L.scale(1.05);
		face_F.center(); face_F.normalize(); face_F.scale(1.05);
		face_B.center(); face_B.normalize(); face_B.scale(1.05);

		// 1/sqrt(2) = 0.7071067811865475
		float d = 0.707106781187f;
		face_F.translate({ 0.0f, -d, 0.0f });
		face_B.translate({ 0.0f,  d,  0.0f });
		face_R.translate({ -d, 0.0f, 0.0f });
		face_L.translate({ d, 0.0f, 0.0f });
		face_U.translate({ 0.0f, 0.0f,  d });
		face_D.translate({ 0.0f, 0.0f, -d });

		float minCoord = 0, maxCoord = 0;
		for (int i = 0; i < face_U.vertices.size(); i++)
		{
			minCoord = (minCoord > face_U.vertices[i].x) ? face_U.vertices[i].x : minCoord;
			minCoord = (minCoord > face_U.vertices[i].y) ? face_U.vertices[i].y : minCoord;
			minCoord = (minCoord > face_U.vertices[i].z) ? face_U.vertices[i].z : minCoord;
			maxCoord = (maxCoord < face_U.vertices[i].x) ? face_U.vertices[i].x : maxCoord;
			maxCoord = (maxCoord < face_U.vertices[i].y) ? face_U.vertices[i].y : maxCoord;
			maxCoord = (maxCoord < face_U.vertices[i].z) ? face_U.vertices[i].z : maxCoord;
		}

		int j = 0;
		for (int i = 0; i < face_U.vertices.size(); i++)
		{
			allVertexDataWithTextureCoords[6 * j + 0] = face_U.vertices[i].x; // vertex positions
			allVertexDataWithTextureCoords[6 * j + 1] = face_U.vertices[i].y;
			allVertexDataWithTextureCoords[6 * j + 2] = face_U.vertices[i].z;
			allVertexDataWithTextureCoords[6 * j + 3] = (face_U.vertices[i].x - minCoord)/(maxCoord - minCoord); // texture coords
			allVertexDataWithTextureCoords[6 * j + 4] = (face_U.vertices[i].y - minCoord)/(maxCoord - minCoord);
			allVertexDataWithTextureCoords[6 * j + 5] = 0.0f; // texture ID
			j++;
		}
		for (int i = 0; i < face_D.vertices.size(); i++)
		{
			allVertexDataWithTextureCoords[6 * j + 0] = face_D.vertices[i].x;
			allVertexDataWithTextureCoords[6 * j + 1] = face_D.vertices[i].y;
			allVertexDataWithTextureCoords[6 * j + 2] = face_D.vertices[i].z;
			allVertexDataWithTextureCoords[6 * j + 3] = (face_D.vertices[i].x - minCoord) / (maxCoord - minCoord); // texture coords
			allVertexDataWithTextureCoords[6 * j + 4] = (face_D.vertices[i].y - minCoord) / (maxCoord - minCoord);
			allVertexDataWithTextureCoords[6 * j + 5] = 1.0f;
			j++;
		}
		for (int i = 0; i < face_F.vertices.size(); i++)
		{
			allVertexDataWithTextureCoords[6 * j + 0] = face_F.vertices[i].x;
			allVertexDataWithTextureCoords[6 * j + 1] = face_F.vertices[i].y;
			allVertexDataWithTextureCoords[6 * j + 2] = face_F.vertices[i].z;
			allVertexDataWithTextureCoords[6 * j + 3] = (face_F.vertices[i].x - minCoord) / (maxCoord - minCoord); // texture coords
			allVertexDataWithTextureCoords[6 * j + 4] = (face_F.vertices[i].z - minCoord) / (maxCoord - minCoord);
			allVertexDataWithTextureCoords[6 * j + 5] = 2.0f;
			j++;
		}
		for (int i = 0; i < face_B.vertices.size(); i++)
		{
			allVertexDataWithTextureCoords[6 * j + 0] = face_B.vertices[i].x;
			allVertexDataWithTextureCoords[6 * j + 1] = face_B.vertices[i].y;
			allVertexDataWithTextureCoords[6 * j + 2] = face_B.vertices[i].z;
			allVertexDataWithTextureCoords[6 * j + 3] = (face_B.vertices[i].x - minCoord) / (maxCoord - minCoord); // texture coords
			allVertexDataWithTextureCoords[6 * j + 4] = (face_B.vertices[i].z - minCoord) / (maxCoord - minCoord);
			allVertexDataWithTextureCoords[6 * j + 5] = 3.0f;
			j++;
		}
		for (int i = 0; i < face_R.vertices.size(); i++)
		{
			allVertexDataWithTextureCoords[6 * j + 0] = face_L.vertices[i].x;
			allVertexDataWithTextureCoords[6 * j + 1] = face_L.vertices[i].y;
			allVertexDataWithTextureCoords[6 * j + 2] = face_L.vertices[i].z;
			allVertexDataWithTextureCoords[6 * j + 3] = (face_L.vertices[i].y - minCoord) / (maxCoord - minCoord); // texture coords
			allVertexDataWithTextureCoords[6 * j + 4] = (face_L.vertices[i].z - minCoord) / (maxCoord - minCoord);
			allVertexDataWithTextureCoords[6 * j + 5] = 4.0f;
			j++;
		}
		for (int i = 0; i < face_L.vertices.size(); i++)
		{
			allVertexDataWithTextureCoords[6 * j + 0] = face_R.vertices[i].x;
			allVertexDataWithTextureCoords[6 * j + 1] = face_R.vertices[i].y;
			allVertexDataWithTextureCoords[6 * j + 2] = face_R.vertices[i].z;
			allVertexDataWithTextureCoords[6 * j + 3] = (face_R.vertices[i].y - minCoord) / (maxCoord - minCoord); // texture coords
			allVertexDataWithTextureCoords[6 * j + 4] = (face_R.vertices[i].z - minCoord) / (maxCoord - minCoord);
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

	m_IndexBuffer = std::move(OpenGLIndexBuffer((uint32_t*)&allIndexData[0], allIndexData.size()));

	// Create the shader that will draw the skybox
	m_TextureShader = std::move(Shader(ParseShader("src/renderer/shader_sources/vertex_shader_skybox.glsl"), ParseShader("src/renderer/shader_sources/fragment_shader_skybox.glsl")));

	m_TextureShader.SearchAndAddUniform("observer_orientation");
	m_TextureShader.SearchAndAddUniform("zoom_level");
	m_TextureShader.SearchAndAddUniform("u_Textures"); // there is a uniform in the fragment shader as well!
	m_TextureShader.Bind();

	int samplers[32];
	for (uint32_t i = 0; i < 32; i++) { samplers[i] = i; }
	m_TextureShader.UploadUniformIntArray("u_Textures", samplers, 32);

//	Load images and create textures out of them
//	uint32_t rID;
	for (uint32_t i = 0; i < texture_files.size(); i++)
	{
		m_Textures.push_back(0);
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

		glCreateTextures(GL_TEXTURE_2D, 1, &m_Textures[i]);
		glTextureStorage2D(m_Textures[i], 1, internalFormat, width, height);
		glTextureParameteri(m_Textures[i], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_Textures[i], GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_Textures[i], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // flags: GL_CLAMP_TO_BORDER, GL_CLAMP_TO_EDGE, GL_REPEAT, GL_MIRRORED_REPEAT
		glTextureParameteri(m_Textures[i], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureSubImage2D(m_Textures[i], 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
//		glActiveTexture(GL_TEXTURE0 + i); // I dont know what this does, progarm seems to work the same way regardless of this line being active or commented out
		glBindTextureUnit(i, m_Textures[i]);
		
		stbi_image_free(data);
	}

	// Unbind
	glBindVertexArray(0);
}


