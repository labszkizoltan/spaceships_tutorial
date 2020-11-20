
#include "helper_visuals.h"
#include "glad/glad.h"



// https://solarianprogrammer.com/2013/05/13/opengl-101-drawing-primitives/
// this will be needed:
// 
// glEnable(GL_PROGRAM_POINT_SIZE);
// gl_PointSize = 10.0; // -> this is a built in variable, that can be set in the vertex shader

Highlighter::Highlighter()
	: m_Shader(ParseShader("src/renderer/shader_sources/vertex_shader_highlighter.glsl"), ParseShader("src/renderer/shader_sources/fragment_shader_highlighter.glsl"))
{
	{
		m_Shader.Bind();
		m_Shader.UploadUniformFloat3("body_translation", glm::vec3(0.0f, 0.0f, 0.0f));
		//	m_Shader.UploadUniformFloatArray("obs_param", );
		m_Shader.UploadUniformFloat("aspect_ratio", 1.0f); // if needed, the aspect ratio will be changeable somewhere else
		m_Shader.UploadUniformFloat3("aColor", glm::vec3(1.0f, 0.0f, 1.0f)); // if needed, the aspect ratio will be changeable somewhere else
	}

}

Highlighter::~Highlighter()
{
	m_Shader.~Shader();
	m_BlueMarkerTriangle.~ColouredMesh();
	m_ForwardDirectionMarker.~ColouredMesh();
	m_BackwardDirectionMarker.~ColouredMesh();
}

Highlighter::Highlighter(const std::string & vertexSrc, const std::string & fragSrc)
{
	m_Shader = Shader(vertexSrc, fragSrc);

	{
		m_Shader.Bind();
		m_Shader.UploadUniformFloat3("body_translation", glm::vec3(0.0f, 0.0f, 0.0f));
		//	m_Shader.UploadUniformFloatArray("obs_param", );
		m_Shader.UploadUniformFloat("aspect_ratio", 1.0f); // if needed, the aspect ratio will be changeable somewhere else
		m_Shader.UploadUniformFloat3("aColor", glm::vec3(1.0f, 0.0f, 1.0f)); // if needed, the aspect ratio will be changeable somewhere else
	}

	std::vector<Vec3D> data1 = { {0,0,0}, {0,0,1}, {-0.05,-0.03,0}, {0,0,1}, {0.05,-0.03,0}, {0,0,1} }; // m_BlueMarkerTriangle
	std::vector<Vec3D> data2 = { {0,0,0}, {1,1,0}, {-0.05,-0.03,0}, {1,1,0}, {0.05,-0.03,0}, {1,1,0} }; // yellow m_ForwardDirectionMarker
	std::vector<Vec3D> data3 = { {0,0,0}, {1,0,0}, {-0.05,-0.03,0}, {1,0,0}, {0.05,-0.03,0}, {1,0,0} }; // red m_BackwardDirectionMarker
	std::vector<Vec3D> data4 = { {0,0,0}, {0.5,0.5,0.5}, {-0.05,-0.03,0}, {0.5,0.5,0.5}, {0.05,-0.03,0}, {0.5,0.5,0.5} }; // grey m_ScreenCenterMarker
	std::vector<uint32_t> indexData = {0,2,1};
	m_BlueMarkerTriangle = std::move(ColouredMesh(data1, indexData));
	m_ForwardDirectionMarker = std::move(ColouredMesh(data2, indexData));
	m_BackwardDirectionMarker = std::move(ColouredMesh(data3, indexData));
	m_ScreenCenterMarker = std::move(ColouredMesh(data4, indexData));
	
}

void Highlighter::DrawMarker(Vec3D location, Vec3D colour)
{
	m_Shader.Bind();
	m_Shader.UploadUniformFloat3("body_translation", location.Glm());
	m_BlueMarkerTriangle.Draw();
}

void Highlighter::DrawAllMarkers(Scene& scene, Player& player)
{
	int bodyIndex = ((int)player.m_BodyPtr - (int)&scene.m_Bodies[0]) / sizeof(Body);
	m_Shader.Bind();
	player.m_Observer.SetObserverInShader(m_Shader);

	for (int i = 0; i < scene.m_Bodies.size(); i++)
	{
		if (scene.m_Integrities[i] > 0.0 && bodyIndex != i)
		{
			m_Shader.UploadUniformFloat3("body_translation", scene.m_Bodies[i].location.Glm());
			m_BlueMarkerTriangle.Draw();
		}
	}
	m_Shader.UploadUniformFloat3("body_translation", player.m_BodyPtr->location.Glm() + player.m_BodyPtr->velocity.Glm());
	m_ForwardDirectionMarker.Draw();
	m_Shader.UploadUniformFloat3("body_translation", player.m_BodyPtr->location.Glm() - player.m_BodyPtr->velocity.Glm());
	m_BackwardDirectionMarker.Draw();
	// this  doesnt work for some reason, the triangle shows up in incorrect places
	m_Shader.UploadUniformFloat3("body_translation", player.m_BodyPtr->location.Glm() + player.m_BodyPtr->orientation.f3.Glm());
	m_ScreenCenterMarker.Draw();
}

void Highlighter::SetAspectRatio(float aspectRatio)
{
	m_Shader.Bind();
	m_Shader.UploadUniformFloat("aspect_ratio", aspectRatio);
}

void Highlighter::SetObserverInShader(Observer& obs)
{
	obs.SetObserverInShader(m_Shader);
}
