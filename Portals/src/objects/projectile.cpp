
#include "projectile.h"
#include "glad/glad.h"

#include "Portals/src/global_constants.h"

// Projectile::Projectile()
// 	: startingPoint(Vec3D()), direction(Vec3D(0,0,1)), length(1.0f), timeToLive(1.0f), owner(nullptr) {}
// 
// 
// Projectile::Projectile(Vec3D sp, Vec3D dir, float len, float ttl, Body* b_ptr)
// 	: startingPoint(sp), direction(dir), length(len), timeToLive(ttl), owner(b_ptr) {}


Projectile::Projectile()
	: startingPoint(Vec3D()), velocity(Vec3D()), orientation(Mat_3D()), length(1.0f), timeToLive(0.0f), owner(nullptr) {}

Projectile::Projectile(Vec3D sp, Vec3D velocity, Mat_3D orient, float len, float ttl, Body* b_ptr)
	: startingPoint(sp), velocity(velocity), orientation(orient), length(len), timeToLive(ttl), owner(b_ptr) {}



ProjectilePool::ProjectilePool()
{
	// set up the shader and initialize its uniforms
	m_ProjectileShader = Shader(ParseShader("src/renderer/shader_sources/vertex_shader.glsl"), ParseShader("src/renderer/shader_sources/fragment_shader.glsl"));
	{
		m_ProjectileShader.Bind();
		m_ProjectileShader.UploadUniformFloat3("projectile_starting_loc", glm::vec3(0.0f, 0.0f, 0.0f));
		m_ProjectileShader.UploadUniformMat3("projectile_orientation", glm::mat3(1.0f));
		m_ProjectileShader.UploadUniformFloat("projectile_length", 1.0f);
		m_ProjectileShader.UploadUniformFloat3("scene_translation", glm::vec3(0.0f, 0.0f, 0.0f));
		m_ProjectileShader.UploadUniformFloat3("observer_translation", glm::vec3(0.0f, 0.0f, 0.0f));
		m_ProjectileShader.UploadUniformMat3("observer_orientation", glm::mat3(1.0f));
		m_ProjectileShader.UploadUniformFloat("zoom_level", 1.0f);
		m_ProjectileShader.UploadUniformFloat("aspect_ratio", 1.0f); // if needed, the aspect ratio will be changeable somewhere else
		m_ProjectileShader.UploadUniformFloat("alpha", 1.0f); // uniform in the vertex shader
	}

	// initialize the data of the beam and create the mesh
	std::vector<Vec3D> vertexAndColorData;
	std::vector<uint32_t> indexData;
	{
		int quadCount = 500;
		float dx = 0.5f, dz = 1.0f/quadCount, gapSize = 0.01f; // gapSize will create a visual gap at the origin of the beam, so at firing it wont cover the view as much
		float windingNumber = 100.0f; // this determines how curved the spiral will be

		vertexAndColorData.resize(2*(2 * quadCount + 2)); // vertex count = 2*quadCount+2
		indexData.resize(6 * quadCount);
		for (int i = 0; i < (2*quadCount + 2); i++)
		{
			float z = (float)(i / 2)*dz;
			vertexAndColorData[2 * i + 0] = (Vec3D(0, 0, gapSize) + Diagonal({1,1,z}) * Vec3D((i % 2 * 2 - 1)*dx*cos((float)(i / 2)*dz*windingNumber), (i % 2 * 2 - 1)*dx*sin((float)(i / 2)*dz*windingNumber), (float)(i / 2)*dz)) / (1.0f + gapSize); // vertex position
			vertexAndColorData[2 * i + 1] = Vec3D(0.1f, 0.99f, 0.3f); // projectile colour
//			vertexAndColorData[2 * i + 1] = Vec3D(0.3f, 0.5f, 0.8f); // projectile colour
		}
		for (int i = 0; i < quadCount; i++)
		{
			indexData[6*i + 0] = 0+2*i;
			indexData[6*i + 1] = 1+2*i;
			indexData[6*i + 2] = 2+2*i;
			indexData[6*i + 3] = 2+2*i;
			indexData[6*i + 4] = 1+2*i;
			indexData[6*i + 5] = 3+2*i;
		}
	}

	m_ProjectileMesh = ColouredMesh(vertexAndColorData, indexData);

	// init objects
	m_Projectiles.resize(m_Size);
	m_IsActive.resize(m_Size);
	for (int i=0; i<m_Size; i++)
		m_IsActive[i] = false;

}

ProjectilePool::~ProjectilePool()
{
}

void ProjectilePool::SetAspectRatio(float aspectRatio)
{
	m_ProjectileShader.Bind();
	m_ProjectileShader.UploadUniformFloat("aspect_ratio", aspectRatio);
}

// Collision with the bodies will be checked on emission, thats why the argument is needed. Returns the index of the body that suffers the hit.
int ProjectilePool::Emit(int ownerIndex, float ownerRange, float timeToLive, std::vector<Body>& bodies, std::vector<float> integrities)
{
	m_CurrentIndex--;
	m_CurrentIndex = m_CurrentIndex < 0 ? (m_Size - 1) : (m_CurrentIndex);

	m_Projectiles[m_CurrentIndex].startingPoint = bodies[ownerIndex].location - bodies[ownerIndex].orientation.f2;
	m_Projectiles[m_CurrentIndex].velocity = bodies[ownerIndex].velocity;
	m_Projectiles[m_CurrentIndex].orientation = bodies[ownerIndex].orientation;
	m_Projectiles[m_CurrentIndex].length = ownerRange;
	m_Projectiles[m_CurrentIndex].timeToLive = timeToLive; // g_TimeToLive;
	m_Projectiles[m_CurrentIndex].owner = &bodies[ownerIndex];
	m_IsActive[m_CurrentIndex] = true;

	// check collisions
	static Vec3D v, v_parallel;
	static float d;
	static float collisionCandidate;
	static int collisionIndex;
	static float beam_width = 1.0f; // make the beam wider, so it is easier to hit a target

	collisionCandidate = m_MaxLength;
	collisionIndex = -1;
	for (int i = 0; i < bodies.size(); i++)
	{
		if (ownerIndex != i && integrities[i] > 0.0f) // check collision only with active bodies
		{
			v = (bodies[i].location - bodies[ownerIndex].location);

			// when the body in question is behind the shooter, dont do anything
			if (v*bodies[ownerIndex].orientation.f3 > 0.0f)
			{
				v_parallel = (v*bodies[ownerIndex].orientation.f3) * bodies[ownerIndex].orientation.f3;
				d = (v - v_parallel).length();

				if (d < bodies[i].scale + beam_width && v_parallel.length() < collisionCandidate)
				{
					m_Projectiles[m_CurrentIndex].length = collisionCandidate = v_parallel.length();
					collisionIndex = i;
				}
			}
		}
	}
	return collisionIndex;
}

void ProjectilePool::Update(float deltaTime)
{
	for (int i = 0; i < m_Projectiles.size(); i++)
	{
		m_Projectiles[i].timeToLive -= deltaTime;
		m_Projectiles[i].startingPoint += deltaTime* m_Projectiles[i].velocity;
		m_IsActive[i] = m_Projectiles[i].timeToLive > 0.0f;
	}
}

void ProjectilePool::Draw(Player player)
{
	glDisable(GL_CULL_FACE);

	static int i, counter=0;

	i = m_CurrentIndex;
	m_ProjectileShader.Bind();
	player.m_Observer.SetObserverInShader(m_ProjectileShader);

	while (m_IsActive[i] && counter < m_Size)
	{
		m_ProjectileShader.UploadUniformFloat3("projectile_starting_loc", m_Projectiles[i].startingPoint.Glm());
		m_ProjectileShader.UploadUniformMat3("projectile_orientation", m_Projectiles[i].orientation.Glm());
		m_ProjectileShader.UploadUniformFloat("projectile_length", m_Projectiles[i].length);
		m_ProjectileShader.UploadUniformFloat("alpha", 0.25f + 0.5f * m_Projectiles[i].timeToLive/g_TimeToLive);
		m_ProjectileMesh.Draw();

		i = (i + 1) % m_Size;
		counter++;
	}
	counter = 0;
	glEnable(GL_CULL_FACE);
}
