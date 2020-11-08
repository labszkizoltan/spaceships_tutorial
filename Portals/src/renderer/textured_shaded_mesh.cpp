

#include "textured_shaded_mesh.h"
#include <fstream>
#include "glad/glad.h"
#include "vendor/stb_image/stb_image.h"



TexturedShadedMesh::TexturedShadedMesh(const std::string& vertexFilepath, const std::string& textureFilepath)
	: m_VertexArray(0), m_VertexBuffer(), m_IndexBuffer(), m_Texture(0)
{
	// reading in the data from the vertex file
	std::vector<float> vertexAndTexcoordData;
	std::vector<uint32_t> indexData;
	{
		std::ifstream myfile(vertexFilepath.c_str());
		int v_count = 0, i_count = 0;

		myfile >> v_count;
		myfile >> i_count;

		vertexAndTexcoordData.resize(v_count * 8); // each vertex consists of 8 floats
		indexData.resize(i_count);

		for (int i = 0; i < v_count * 8; i++)
			myfile >> vertexAndTexcoordData[i];

		for (int i = 0; i < i_count; i++)
			myfile >> indexData[i];

		myfile.close();
	}

	// Create the Vertex/Index array/buffers 
	glGenVertexArrays(1, &m_VertexArray);
	glBindVertexArray(m_VertexArray);

	m_VertexBuffer = std::move(OpenGLVertexBuffer((float*)&vertexAndTexcoordData[0], vertexAndTexcoordData.size() * sizeof(float)));
	m_VertexBuffer.Bind();
	m_VertexBuffer.SetLayout({
		{ShaderDataType::Float3, "aPos"},
		{ShaderDataType::Float3, "aSurfaceNormal"},
		{ShaderDataType::Float2, "aTexCoord"}
		});

	m_IndexBuffer = std::move(OpenGLIndexBuffer((uint32_t*)&indexData[0], indexData.size()));

	// Create the texture
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			data = stbi_load(textureFilepath.c_str(), &width, &height, &channels, 0);
		}
		if (!data) { std::cout << "Failed to load image: " << textureFilepath; }

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

		glCreateTextures(GL_TEXTURE_2D, 1, &m_Texture);
		glTextureStorage2D(m_Texture, 1, internalFormat, width, height);
		glTextureParameteri(m_Texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_Texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_Texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // flags: GL_CLAMP_TO_BORDER, GL_CLAMP_TO_EDGE, GL_REPEAT, GL_MIRRORED_REPEAT
		glTextureParameteri(m_Texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureSubImage2D(m_Texture, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
		glBindTextureUnit(6, m_Texture); // texture units 0-5 are used by the skybox

		stbi_image_free(data);
	}
}

// move constructor
TexturedShadedMesh::TexturedShadedMesh(TexturedShadedMesh && other) noexcept
{
	glDeleteVertexArrays(1, &m_VertexArray);
	m_VertexBuffer.~OpenGLVertexBuffer();
	m_IndexBuffer.~OpenGLIndexBuffer();
	glDeleteTextures(1, &m_Texture);

	m_VertexArray = other.m_VertexArray;
	m_VertexBuffer = std::move(other.m_VertexBuffer); // this move operation should transfer the renderer Id of the temporary object, and zero it out afterwards, so upon destruction nothing will be deleted
	m_IndexBuffer = std::move(other.m_IndexBuffer); // same as above
	m_Texture = other.m_Texture;

	other.m_VertexArray = 0;
	other.m_Texture = 0;
}

// move assignment
TexturedShadedMesh & TexturedShadedMesh::operator=(TexturedShadedMesh && other) noexcept
{
	if (this != &other)
	{
		glDeleteVertexArrays(1, &m_VertexArray);
		m_VertexBuffer.~OpenGLVertexBuffer();
		m_IndexBuffer.~OpenGLIndexBuffer();
		glDeleteTextures(1, &m_Texture);

		m_VertexArray = other.m_VertexArray;
		m_VertexBuffer = std::move(other.m_VertexBuffer); // this move operation should transfer the renderer Id of the temporary object, and zero it out afterwards, so upon destruction nothing will be deleted
		m_IndexBuffer = std::move(other.m_IndexBuffer); // same as above
		m_Texture = other.m_Texture;

		other.m_VertexArray = 0;
		other.m_Texture = 0;
	}

	return *this;
}

TexturedShadedMesh::~TexturedShadedMesh()
{
	glDeleteVertexArrays(1, &m_VertexArray);
	m_VertexBuffer.~OpenGLVertexBuffer();
	m_IndexBuffer.~OpenGLIndexBuffer();
	glDeleteTextures(1, &m_Texture);
}

void TexturedShadedMesh::Draw()
{
	glBindVertexArray(m_VertexArray);
	m_VertexBuffer.SetLayout();
	m_IndexBuffer.Bind();
	glBindTextureUnit(6, m_Texture);
	glDrawElements(GL_TRIANGLES, m_IndexBuffer.m_Count, GL_UNSIGNED_INT, nullptr);
}





