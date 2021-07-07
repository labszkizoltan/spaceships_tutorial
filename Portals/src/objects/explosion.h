#pragma once

#include "Portals/src/utils/Vector_3D.h"
#include "Portals/src/utils/Matrix_3D.h"
#include "Portals/src/renderer/textured_mesh.h"
#include "Portals/src/objects/body.h"
#include "Portals/src/controls/player.h"
#include "Portals/src/renderer/coloured_mesh.h"

#include "Portals/src/global_constants.h"



struct Explosion
{
	Explosion();
	Explosion(Vec3D sp, Vec3D velocity, float size, float ttl);

	Vec3D startingPoint;
	Vec3D velocity;
	float initial_size;
	float timeToLive = g_ExplosionTimeToLive;
};


class ExplosionPool
{
public:
	ExplosionPool();
	~ExplosionPool();

	void SetAspectRatio(float aspectRatio);

	void Emit(Explosion explosion);
	void Update(float deltaTime);
	void Draw(Player player);

private:
	uint32_t m_Size = 1000;
	int m_CurrentIndex = m_Size - 1;

	std::vector<Explosion> m_Explosions;
	std::vector<bool> m_IsActive;

	Shader m_ExplosionShader; // (ParseShader("src/renderer/shader_sources/vertex_shader_textured.glsl"), ParseShader("src/renderer/shader_sources/fragment_shader_textured.glsl"));
	ColouredMesh m_ExplosionMesh;
};












