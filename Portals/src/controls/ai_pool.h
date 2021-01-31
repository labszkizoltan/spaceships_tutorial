#pragma once

#include "ai.h"
#include "Portals/src/utils/Vector_3D.h"
#include "Portals/src/utils/Matrix_3D.h"

#include "Portals/src/global_constants.h"

class AIPool
{
public:

	AIPool(const std::string& filename, Scene& scene);
	~AIPool() = default;

	void Update(Scene& scene, float timestep);


private:
	std::vector<AI> m_AISet;
//	std::vector<int> m_Teams;
	std::vector<AIAccelerationScript> m_AccelScripts;

};













