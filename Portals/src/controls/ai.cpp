
#include "ai.h"
#include "Portals/src/utils/Vector_3D.h"
#include "Portals/src/utils/Matrix_3D.h"

#include "Portals/src/global_constants.h"

AI::AI() //: m_BodyPtr(nullptr), m_TargetBody(nullptr), m_ShotTimer(0.0f), m_ShotCooldown(1.0f) {}
	: m_BodyPtr(nullptr), m_TargetBody(nullptr), m_ShotTimer(0.0f), m_ShotCooldown(1.0f), m_Acceleration(g_AIDefaultAcceleration), m_TurnRate(g_AIDefaultTurnRate), m_AttractionRange(g_AIDefaultAttractionRange), m_RepulsionRange(g_AIDefaultRepulsionRange)
{}

AI::AI(Body* bodyPtr, Body* targetPtr, float cooldown, float accel, float turnRate, float attrRange, float repRange)
	: m_BodyPtr(bodyPtr), m_TargetBody(targetPtr), m_ShotTimer(0.0f), m_ShotCooldown(cooldown), m_Acceleration(accel), m_TurnRate(turnRate), m_AttractionRange(attrRange), m_RepulsionRange(repRange)
{}


AI::~AI()
{
	m_BodyPtr = nullptr;
	m_TargetBody = nullptr;
}

void AI::Update(Scene& scene, float timestep)
{
	int bodyIndex = ((int)m_BodyPtr - (int)&scene.m_Bodies[0]) / sizeof(Body);
	if (scene.m_Integrities[bodyIndex] <= 0.0f) { return; } // check if the body is still valid, or destroyed

	Vec3D dr = m_TargetBody->location - m_BodyPtr->location;
	Vec3D dv = m_TargetBody->velocity - m_BodyPtr->velocity;
	Vec3D z = m_BodyPtr->orientation.f3;

	// this part could be replaced with a function pointer, so custom behaviour could be implemented by putting different functions here
	if (dr.length() > m_AttractionRange)
	{
		m_BodyPtr->AccelerateDir((m_Acceleration * timestep / dr.length()) * dr);
	}
	else if (dr.length() < m_RepulsionRange)
	{
		m_BodyPtr->AccelerateDir(((-1.0f) * m_Acceleration * timestep / dr.length()) * dr);
	}

	// turn towards the target:
	Vec3D n = CrossProduct(z, dr); // no need to normalize the vector, since the Rotation matrix generator function does that
	float phi = acos(z*dr / (z.length()*dr.length()));
	phi = !std::isfinite(phi) ? 0.0f : (phi > (m_TurnRate*timestep) ? (m_TurnRate*timestep) : phi);
	m_BodyPtr->Turn(n, phi);

	// Shoot
	if (dr.length() < g_AIDefaultWeaponRange && m_ShotTimer > m_ShotCooldown && phi < 0.05f) // PARAMETER AIShootPrecision
	{
		scene.OnShoot(m_BodyPtr, g_AIDefaultWeaponRange); m_ShotTimer = 0.0f;
	}

	m_ShotTimer += timestep;
}

void AI::Retarget(std::vector<Body> bodies)
{
}
