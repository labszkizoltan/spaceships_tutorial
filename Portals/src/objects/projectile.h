#pragma once

#include "Portals/src/utils/Vector_3D.h"
#include "Portals/src/utils/Matrix_3D.h"
#include "Portals/src/renderer/textured_mesh.h"
#include "Portals/src/objects/body.h"
#include "Portals/src/controls/player.h"
#include "Portals/src/renderer/coloured_mesh.h"

#include "Portals/src/global_constants.h"

struct Projectile
{
	Projectile();
//	Projectile(Vec3D sp, Vec3D dir, float len, float ttl, Body* b_ptr);
	Projectile(Vec3D sp, Vec3D velocity, Mat_3D orient, float len, float ttl, Body* b_ptr);

	Vec3D startingPoint;
	Vec3D velocity;
	Mat_3D orientation;
	float length;
	float timeToLive = g_TimeToLive;
	Body* owner; // pointer to the body that shot the projectile, this will be used to avoid detecting collisions with the owner
};


class ProjectilePool
{
public:
	ProjectilePool();
	~ProjectilePool();

	void SetAspectRatio(float aspectRatio);

	// owner should point to an element in bodies!
	int Emit(int ownerIndex, float ownerRange, float timeToLive, std::vector<Body>& bodies, std::vector<float> integrities); // Collision with the bodies will be checked on emission, thats why the argument is needed. Returns the index of the body that suffers the hit.
	void Update(float deltaTime);
	void Draw(Player player);

private:
	uint32_t m_Size = 1000;
	int m_CurrentIndex = m_Size-1;
	float m_MaxLength = g_MaxLength; // ShootRange PARAMETER

	std::vector<Projectile> m_Projectiles;
	std::vector<bool> m_IsActive;

	Shader m_ProjectileShader; // (ParseShader("src/renderer/shader_sources/vertex_shader_textured.glsl"), ParseShader("src/renderer/shader_sources/fragment_shader_textured.glsl"));
	ColouredMesh m_ProjectileMesh;
};






