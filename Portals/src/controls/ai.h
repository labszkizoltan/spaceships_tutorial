#pragma once

#include "Portals/src/objects/body.h"
#include "Portals/src/renderer/scene.h"

#include "Portals/src/global_constants.h"

typedef Vec3D(*AIAccelerationScript) (std::vector<Body>& bodies, float timestep, AI& ai);

// forward declaration, so the functions can be used in places where this header is included
Vec3D AIAccelerationFunction_basic(std::vector<Body>& bodies, float timestep, AI& ai);
Vec3D AIAccelerationFunction_advanced(std::vector<Body>& bodies, float timestep, AI& ai);

struct AIProperties
{
	float m_ShotCooldown = g_AIDefaultCooldownTime;
	float m_Acceleration = g_AIDefaultAcceleration;
	float m_TurnRate = g_AIDefaultTurnRate;
	float m_AttractionRange = g_AIDefaultAttractionRange;
	float m_RepulsionRange = g_AIDefaultRepulsionRange; // all of these things could go in a struct
};


class AI
{
public:
	AI();
	AI(Body* bodyPtr, Body* targetPtr, AIProperties props);
	~AI();

	void Update(Scene& scene, float timestep, AIAccelerationScript accelFunc); // this may be the function where the shoot/accelerate command are issued
//	void Retarget(std::vector<Body>& bodies); // the body vector is not enough on its own, since it doesnt contain the integrity info
	void Retarget(Scene& scene);
	


private:
	Body* m_BodyPtr = nullptr;
	Body* m_TargetBody = nullptr;
	float m_ShotTimer = 0.0f;

	AIProperties m_Props;

	// also there could be a few predefined function representing the behaviour and those functions would be called from the Update method

	friend Vec3D AIAccelerationFunction_basic(std::vector<Body>& bodies, float timestep, AI& ai);
	friend Vec3D AIAccelerationFunction_advanced(std::vector<Body>& bodies, float timestep, AI& ai);

	
};


class AIPool
{
public:
	AIPool();
	AIPool(std::vector<AI> AISet);
	~AIPool();

	void Update(Scene& scene, float timestep); // this may be the function where the shoot/accelerate command are issued
	void Retarget(Scene& scene);

private:
	std::vector<AI> m_AISet;


};



