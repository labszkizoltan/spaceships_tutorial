#include "explosion.h"
#include "glad/glad.h"

#include "Portals/src/global_constants.h"



Explosion::Explosion()
	: startingPoint(Vec3D()), velocity(Vec3D()), timeToLive(0.0f) {}

Explosion::Explosion(Vec3D sp, Vec3D velocity, float size, float ttl)
	: startingPoint(sp), velocity(velocity), initial_size(size), timeToLive(ttl) {}




ExplosionPool::ExplosionPool()
{
	// set up the shader and initialize its uniforms
	m_ExplosionShader = Shader(ParseShader("src/renderer/shader_sources/vertex_shader_explosion.glsl"), ParseShader("src/renderer/shader_sources/fragment_shader_explosion.glsl"));
	{
		m_ExplosionShader.Bind();
		m_ExplosionShader.UploadUniformFloat3("starting_loc", glm::vec3(0.0f, 0.0f, 0.0f));
		m_ExplosionShader.UploadUniformMat3("orientation", glm::mat3(1.0f));
		m_ExplosionShader.UploadUniformFloat("scale", 1.0f);
		m_ExplosionShader.UploadUniformFloat3("scene_translation", glm::vec3(0.0f, 0.0f, 0.0f));
		m_ExplosionShader.UploadUniformFloat3("observer_translation", glm::vec3(0.0f, 0.0f, 0.0f));
		m_ExplosionShader.UploadUniformMat3("observer_orientation", glm::mat3(1.0f));
		m_ExplosionShader.UploadUniformFloat("zoom_level", 1.0f);
		m_ExplosionShader.UploadUniformFloat("aspect_ratio", 1.0f); // if needed, the aspect ratio will be changeable somewhere else
		m_ExplosionShader.UploadUniformFloat("alpha", 1.0f); // uniform in the vertex shader
	}

	// initialize mesh from the assets folder -> CHANGE FILEPATH TO RELATIVE PATH !!!
	m_ExplosionMesh = ColouredMesh("D:/cpp_codes/26_portals/Portals/assets/coloured_meshes/Sphere_10_orange.txt");

	// init objects
	m_Explosions.resize(m_Size);
	m_IsActive.resize(m_Size);
	for (int i = 0; i < m_Size; i++)
		m_IsActive[i] = false;
}

ExplosionPool::~ExplosionPool()
{
}

void ExplosionPool::SetAspectRatio(float aspectRatio)
{
	m_ExplosionShader.Bind();
	m_ExplosionShader.UploadUniformFloat("aspect_ratio", aspectRatio);
}

void ExplosionPool::Emit(Explosion explosion)
{
	m_CurrentIndex--;
	m_CurrentIndex = m_CurrentIndex < 0 ? (m_Size - 1) : (m_CurrentIndex);

	m_Explosions[m_CurrentIndex] = explosion;
	m_IsActive[m_CurrentIndex] = true;
}

void ExplosionPool::Update(float deltaTime)
{
	for (int i = 0; i < m_Explosions.size(); i++)
	{
		m_Explosions[i].timeToLive -= deltaTime;
		m_Explosions[i].startingPoint += deltaTime * m_Explosions[i].velocity;
		m_IsActive[i] = m_Explosions[i].timeToLive > 0.0f;
	}
}

void ExplosionPool::Draw(Player player)
{
//	glDisable(GL_CULL_FACE);
	// turn off writing to the depth buffer
	glDepthMask(GL_FALSE);

	int i = m_CurrentIndex, counter = 0;
	
	m_ExplosionShader.Bind();
	player.m_Observer.SetObserverInShader(m_ExplosionShader);

	while (m_IsActive[i] && counter < m_Size)
	{
		m_ExplosionShader.UploadUniformFloat3("starting_loc", m_Explosions[i].startingPoint.Glm());
		m_ExplosionShader.UploadUniformFloat("alpha", m_Explosions[i].timeToLive / g_ExplosionTimeToLive);
		float lambda = m_Explosions[i].timeToLive / g_ExplosionTimeToLive;
		float explosion_size = m_Explosions[i].initial_size * (lambda + g_ExplosionFinalSizeMultiplier * (1-lambda)); // 5 controls the final size
		m_ExplosionShader.UploadUniformFloat("scale", explosion_size);
		m_ExplosionMesh.Draw();

		i = (i + 1) % m_Size;
		counter++;
	}
	counter = 0;
//	glEnable(GL_CULL_FACE);
	// turn back on again
	glDepthMask(GL_TRUE);
}
