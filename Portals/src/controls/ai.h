#pragma once

#include "Portals/src/objects/body.h"
#include "Portals/src/renderer/scene.h"

#include "Portals/src/global_constants.h"

typedef Vec3D(*AIAccelerationScript) (std::vector<Body>& bodies, float timestep, AI& ai);

// forward declaration, so the functions can be used in places where this header is included
Vec3D AIAccelerationFunction_basic(std::vector<Body>& bodies, float timestep, AI& ai);
Vec3D AIAccelerationFunction_advanced(std::vector<Body>& bodies, float timestep, AI& ai);
Vec3D AIAccelerationFunction_stationaryPlatform(std::vector<Body>& bodies, float timestep, AI& ai);

// these quantities should be tied to the object (as they are mostly the properties of the ships), not the controlling ai
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
	AI(Body* bodyPtr, Body* targetPtr, AIProperties props, uint32_t team);
	~AI();

	void Update(Scene& scene, float timestep, AIAccelerationScript accelFunc); // this may be the function where the shoot/accelerate command are issued
//	void Retarget(std::vector<Body>& bodies); // the body vector is not enough on its own, since it doesnt contain the integrity info
	void Retarget(Scene& scene);
	


private:
	Body* m_BodyPtr = nullptr;
	Body* m_TargetBody = nullptr;
	float m_ShotTimer = 0.0f;
	uint32_t m_TeamID = 0; // bodies not controlled by an ai, should still belong to a team (if nothing else, a neutral default team) so probably this should be somewhere else

	AIProperties m_Props;

	// also there could be a few predefined function representing the behaviour and those functions would be called from the Update method

	friend Vec3D AIAccelerationFunction_basic(std::vector<Body>& bodies, float timestep, AI& ai);
	friend Vec3D AIAccelerationFunction_advanced(std::vector<Body>& bodies, float timestep, AI& ai);

	
};




