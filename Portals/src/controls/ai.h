#pragma once

#include "Portals/src/objects/body.h"
#include "Portals/src/renderer/scene.h"

#include "Portals/src/global_constants.h"


class AI
{
public:
	AI();
	AI(Body* bodyPtr, Body* targetPtr, float cooldown, float accel, float turnRate, float attrRange, float repRange);
	~AI();

	void Update(Scene& scene, float timestep); // this may be the function where the shoot/accelerate command are issued
	void Retarget(std::vector<Body> bodies);


private:
	Body* m_BodyPtr;
	Body* m_TargetBody;
	float m_ShotTimer, m_ShotCooldown;
	float m_Acceleration = g_AIDefaultAcceleration;
	float m_TurnRate = g_AIDefaultTurnRate;
	float m_AttractionRange = g_AIDefaultAttractionRange;
	float m_RepulsionRange = g_AIDefaultRepulsionRange; // all of these things could go in a struct

	// also there could be a few predefined function representing the behaviour and those functions would be called from the Update method
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



