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
//	glDeleteVertexArrays(1, &m_VertexArray);
	m_VertexBuffer.~OpenGLVertexBuffer();
	m_IndexBuffer.~OpenGLIndexBuffer();
//	m_TextureShader.~Shader();
}

void Skybox::Draw(Observer obs)
{
	glDisable(GL_DEPTH_TEST);



	glEnable(GL_DEPTH_TEST);
}


void Skybox::Init(std::vector<std::string> texture_files, int planeResolution)
{
	glCreateVertexArrays(1, &m_VertexArray);
	glBindVertexArray(m_VertexArray);


//	Create Vertex Data with texture coordinates
	Shape3D face_U = CreatePlane(planeResolution, planeResolution);
	face_U.center();
	face_U.normalize();

	Shape3D face_D = face_U;
	Shape3D face_F = face_U;
	Shape3D face_B = face_U;
	Shape3D face_L = face_U;
	Shape3D face_R = face_U;
	std::vector<float> allVertexDataWithTextureCoords;
	allVertexDataWithTextureCoords.resize(6 * 5 * face_U.vertices.size());

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
			allVertexDataWithTextureCoords[5 * j + 0] = face_U.vertices[0].x;
			allVertexDataWithTextureCoords[5 * j + 1] = face_U.vertices[0].y;
			allVertexDataWithTextureCoords[5 * j + 2] = face_U.vertices[0].z;
			allVertexDataWithTextureCoords[5 * j + 3] = (float)(j % (planeResolution +1)/(float)planeResolution); // texture coords
			allVertexDataWithTextureCoords[5 * j + 4] = (float)(j / (planeResolution +1)/(float)planeResolution);
			j++;
		}
		for (int i = 0; i < face_D.vertices.size(); i++)
		{
			allVertexDataWithTextureCoords[5 * j + 0] = face_D.vertices[0].x;
			allVertexDataWithTextureCoords[5 * j + 1] = face_D.vertices[0].y;
			allVertexDataWithTextureCoords[5 * j + 2] = face_D.vertices[0].z;
			allVertexDataWithTextureCoords[5 * j + 3] = (float)(j % (planeResolution + 1) / (float)planeResolution); // texture coords
			allVertexDataWithTextureCoords[5 * j + 4] = (float)(j / (planeResolution + 1) / (float)planeResolution);
			j++;
		}
		for (int i = 0; i < face_F.vertices.size(); i++)
		{
			allVertexDataWithTextureCoords[5 * j + 0] = face_F.vertices[0].x;
			allVertexDataWithTextureCoords[5 * j + 1] = face_F.vertices[0].y;
			allVertexDataWithTextureCoords[5 * j + 2] = face_F.vertices[0].z;
			allVertexDataWithTextureCoords[5 * j + 3] = (float)(j % (planeResolution + 1) / (float)planeResolution); // texture coords
			allVertexDataWithTextureCoords[5 * j + 4] = (float)(j / (planeResolution + 1) / (float)planeResolution);
			j++;
		}
		for (int i = 0; i < face_B.vertices.size(); i++)
		{
			allVertexDataWithTextureCoords[5 * j + 0] = face_B.vertices[0].x;
			allVertexDataWithTextureCoords[5 * j + 1] = face_B.vertices[0].y;
			allVertexDataWithTextureCoords[5 * j + 2] = face_B.vertices[0].z;
			allVertexDataWithTextureCoords[5 * j + 3] = (float)(j % (planeResolution + 1) / (float)planeResolution); // texture coords
			allVertexDataWithTextureCoords[5 * j + 4] = (float)(j / (planeResolution + 1) / (float)planeResolution);
			j++;
		}
		for (int i = 0; i < face_L.vertices.size(); i++)
		{
			allVertexDataWithTextureCoords[5 * j + 0] = face_L.vertices[0].x;
			allVertexDataWithTextureCoords[5 * j + 1] = face_L.vertices[0].y;
			allVertexDataWithTextureCoords[5 * j + 2] = face_L.vertices[0].z;
			allVertexDataWithTextureCoords[5 * j + 3] = (float)(j % (planeResolution + 1) / (float)planeResolution); // texture coords
			allVertexDataWithTextureCoords[5 * j + 4] = (float)(j / (planeResolution + 1) / (float)planeResolution);
			j++;
		}
		for (int i = 0; i < face_R.vertices.size(); i++)
		{
			allVertexDataWithTextureCoords[5 * j + 0] = face_R.vertices[0].x;
			allVertexDataWithTextureCoords[5 * j + 1] = face_R.vertices[0].y;
			allVertexDataWithTextureCoords[5 * j + 2] = face_R.vertices[0].z;
			allVertexDataWithTextureCoords[5 * j + 3] = (float)(j % (planeResolution + 1) / (float)planeResolution); // texture coords
			allVertexDataWithTextureCoords[5 * j + 4] = (float)(j / (planeResolution + 1) / (float)planeResolution);
			j++;
		}

	}

	// do this after the vertex data has been initialized
	m_VertexBuffer = OpenGLVertexBuffer((float*)&allVertexDataWithTextureCoords[0], allVertexDataWithTextureCoords.size() * sizeof(float));
	m_VertexBuffer.SetLayout({
		{ShaderDataType::Float3, "aPos"},
		{ShaderDataType::Float2, "aTexCoord"}
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


	// Unbind
	glBindVertexArray(0);
}


