
#include "Portals/src/controls/ai_pool.h"

AIPool::AIPool(const std::string & filename, Scene& scene)
{
	std::string line;
	std::vector<std::string> file_in_memory;
	std::ifstream myfile(filename);

	while (getline(myfile, line))
	{
		if (!line.empty() && line.find("//")) { file_in_memory.push_back(line); } // leave out empty lines and the ones that begin with "//"
	}
	myfile.close();

	int i = 0;
	while (i < file_in_memory.size())
	{
		if (file_in_memory[i].find("{") != -1)
		{
			std::string tmp; // this just holds the search word, the value will come after this
			int bodyIdx, targetIdx, team;
			std::string scriptName;
			AIProperties defaultProps;
			AIAccelerationScript tmpScript;

			while (file_in_memory[i].find("}") == -1)
			{
				if (file_in_memory[i].find("bodyIndex") != -1)
				{
					std::stringstream ss(file_in_memory[i]);
					ss >> tmp;
					ss >> bodyIdx;
				}
				if (file_in_memory[i].find("targetIndex") != -1)
				{
					std::stringstream ss(file_in_memory[i]);
					ss >> tmp;
					ss >> targetIdx;
				}
				if (file_in_memory[i].find("team") != -1)
				{
					std::stringstream ss(file_in_memory[i]);
					ss >> tmp;
					ss >> team;
				}
				if (file_in_memory[i].find("scriptName") != -1)
				{
					std::stringstream ss(file_in_memory[i]);
					ss >> tmp;
					ss >> scriptName;
					if (scriptName == "AIAccelerationFunction_basic") { tmpScript = AIAccelerationFunction_basic; }
					if (scriptName == "AIAccelerationFunction_advanced") { tmpScript = AIAccelerationFunction_advanced; }
					if (scriptName == "AIAccelerationFunction_stationaryPlatform") { tmpScript = AIAccelerationFunction_stationaryPlatform; }
				}

				if (file_in_memory[i].find("ShotCooldown") != -1)
				{
					std::stringstream ss(file_in_memory[i]);
					ss >> tmp;
					ss >> defaultProps.m_ShotCooldown;
				}
				if (file_in_memory[i].find("Acceleration") != -1)
				{
					std::stringstream ss(file_in_memory[i]);
					ss >> tmp;
					ss >> defaultProps.m_Acceleration;
				}
				if (file_in_memory[i].find("TurnRate") != -1)
				{
					std::stringstream ss(file_in_memory[i]);
					ss >> tmp;
					ss >> defaultProps.m_TurnRate;
				}
				if (file_in_memory[i].find("AttractionRange") != -1)
				{
					std::stringstream ss(file_in_memory[i]);
					ss >> tmp;
					ss >> defaultProps.m_AttractionRange;
				}
				if (file_in_memory[i].find("RepulsionRange") != -1)
				{
					std::stringstream ss(file_in_memory[i]);
					ss >> tmp;
					ss >> defaultProps.m_RepulsionRange;
				}

				i++;
			}

			m_AISet.push_back(AI(scene.GetBodyPtr(bodyIdx), scene.GetBodyPtr(targetIdx), defaultProps, team));
			m_AccelScripts.push_back(tmpScript);
		}
		i++;
	}

}

void AIPool::Update(Scene& scene, float timestep)
{
	for (int i=0; i<m_AISet.size(); i++)
	{
		m_AISet[i].Update(scene, timestep, m_AccelScripts[i]);
	}
}
