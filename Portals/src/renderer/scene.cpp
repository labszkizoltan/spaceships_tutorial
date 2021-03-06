
#include "scene.h"

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <sstream>

#include <glad/glad.h>

#include "Portals/src/utils/Matrix_3D.h"
#include "Portals/src/objects/body.h"
#include "Portals/src/renderer/textured_shaded_mesh.h"
#include "Portals/src/renderer/shader.h"


enum class ReadMode { none, skybox, bodies, type_file_lookup, texture_lookup };

void SetMode(ReadMode& read_mode, std::string string_input)
{
	if (string_input.find("skybox") != -1) { read_mode = ReadMode::skybox; }
	if (string_input.find("bodies") != -1) { read_mode = ReadMode::bodies; }
	if (string_input.find("type_file_lookup") != -1) { read_mode = ReadMode::type_file_lookup; }
	if (string_input.find("texture_lookup") != -1) { read_mode = ReadMode::texture_lookup; }
}

struct ParsedInput
{
	std::map<std::string, std::string> skybox_files;
	std::vector<std::string> body_types;
	std::vector<Body> bodies;
	std::vector<float> integrities;
	std::map<std::string, std::string> mesh_files;
	std::map<std::string, std::string> mesh_textures;
};

// this might worth implementing:
bool ParsedInputIntegrityChecker(ParsedInput& parsed_input)
{
	return true;
}

void ParseSceneDefinitionFile(ParsedInput& parsed_input, const std::string & filename)
{
	std::string line;
	std::vector<std::string> file_in_memory;
	std::ifstream myfile(filename);

	while (getline(myfile, line))
	{
		if (!line.empty() && line.find("//")) { file_in_memory.push_back(line); } // leave out empty lines and the ones that begin with "//"
	}
	myfile.close();

	ReadMode read_mode = ReadMode::none;

	for (int i = 0; i < file_in_memory.size(); i++)
	{
		if (file_in_memory[i].find("#") != -1) { SetMode(read_mode, file_in_memory[i]); }

		if (read_mode == ReadMode::skybox)
		{
			i++;
			while (i < file_in_memory.size() && file_in_memory[i].find("#") == -1)
			{
				std::stringstream ss(file_in_memory[i]);
				std::string key, value;
				ss >> key;
				ss >> value;
				parsed_input.skybox_files[key] = value;
				i++;
			}
			read_mode == ReadMode::none;
			i--;
		}

		if (read_mode == ReadMode::type_file_lookup)
		{
			i++;
			while (i < file_in_memory.size() && file_in_memory[i].find("#") == -1)
			{
				std::stringstream ss(file_in_memory[i]);
				std::string key, value;
				ss >> key;
				ss >> value;
				parsed_input.mesh_files[key] = value;
				i++;
			}
			read_mode == ReadMode::none;
			i--;
		}

		if (read_mode == ReadMode::texture_lookup)
		{
			i++;
			while (i < file_in_memory.size() && file_in_memory[i].find("#") == -1)
			{
				std::stringstream ss(file_in_memory[i]);
				std::string key, value;
				ss >> key;
				ss >> value;
				parsed_input.mesh_textures[key] = value;
				i++;
			}
			read_mode == ReadMode::none;
			i--;
		}

		if (read_mode == ReadMode::bodies)
		{
			i++;
			while (i < file_in_memory.size() && file_in_memory[i].find("#") == -1)
			{
				if (file_in_memory[i].find("{") != -1)
				{
					std::string tmp;
					std::string body_type;
					Body body_to_add;
					while (file_in_memory[i].find("}") == -1)
					{
						if (file_in_memory[i].find("type") != -1)
						{
							std::stringstream ss(file_in_memory[i]);
							ss >> tmp;
							ss >> body_type;
							parsed_input.body_types.push_back(body_type);
						}
						if (file_in_memory[i].find("position") != -1)
						{
							std::stringstream ss(file_in_memory[i]);
							ss >> tmp;
							ss >> body_to_add.location.x;
							ss >> body_to_add.location.y;
							ss >> body_to_add.location.z;
						}
						if (file_in_memory[i].find("rotation") != -1)
						{
							std::stringstream ss(file_in_memory[i]);
							ss >> tmp;
							float angle;
							Vec3D direction;
							ss >> angle;
							ss >> direction.x; ss >> direction.y; ss >> direction.z;
							body_to_add.orientation = Rotation(angle, direction);
						}
						if (file_in_memory[i].find("scale") != -1)
						{
							std::stringstream ss(file_in_memory[i]);
							ss >> tmp;
							ss >> body_to_add.scale;
						}
						if (file_in_memory[i].find("velocity") != -1)
						{
							std::stringstream ss(file_in_memory[i]);
							ss >> tmp;
							ss >> body_to_add.velocity.x;
							ss >> body_to_add.velocity.y;
							ss >> body_to_add.velocity.z;
						}
						if (file_in_memory[i].find("angularVelocity") != -1)
						{
							std::stringstream ss(file_in_memory[i]);
							ss >> tmp;
							ss >> body_to_add.angularVelocity.x;
							ss >> body_to_add.angularVelocity.y;
							ss >> body_to_add.angularVelocity.z;
						}
						if (file_in_memory[i].find("integrity") != -1)
						{
							std::stringstream ss(file_in_memory[i]);
							ss >> tmp;
							ss >> tmp;
							parsed_input.integrities.push_back(std::atof(tmp.c_str()));
						}
						if (file_in_memory[i].find("mass") != -1)
						{
							std::stringstream ss(file_in_memory[i]);
							ss >> tmp;
							ss >> body_to_add.mass;
						}
						i++;
					}
					parsed_input.bodies.push_back(body_to_add);
				}
				i++;
			}

			read_mode == ReadMode::none;
			i--;
		}

	}

}

Scene::Scene(const std::string & filename)
{
	ParsedInput parsed_input;
	ParseSceneDefinitionFile(parsed_input, filename);
	std::cout << "Scene::Scene(): input file parsed\n";

	if (!ParsedInputIntegrityChecker(parsed_input)) { std::cout << "Scene definition Integrity check failed\n"; return; }
	
	// Create Skybox
	std::vector<std::string> skybox_texture_files; skybox_texture_files.resize(6);
	skybox_texture_files[0] = parsed_input.skybox_files["up"];
	skybox_texture_files[1] = parsed_input.skybox_files["down"];
	skybox_texture_files[2] = parsed_input.skybox_files["front"];
	skybox_texture_files[3] = parsed_input.skybox_files["back"];
	skybox_texture_files[4] = parsed_input.skybox_files["left"];
	skybox_texture_files[5] = parsed_input.skybox_files["right"];

	// Skybox class has a default constructor that creates a valid skybox, so at this point that already valid skybox is deleted and replaced be the ont that is defined in the config file
	// so dont be surptised when vertex/index buffers are destroyed with non 0 id-s
	m_Skybox = std::move(Skybox(skybox_texture_files, 10));


	// Initialize objects
	std::vector<std::string> bodyTypes;
		bodyTypes.resize(parsed_input.body_types.size());
	m_Bodies.resize(parsed_input.bodies.size());
	m_Integrities.resize(parsed_input.bodies.size());
	for (int i = 0; i < parsed_input.body_types.size(); i++)
	{
		bodyTypes[i] = parsed_input.body_types[i];
		m_Bodies[i] = parsed_input.bodies[i]; 
		m_Integrities[i] = parsed_input.integrities[i];
	}
	m_Accelerations.resize(m_Bodies.size());
	m_AngularAccelerations.resize(m_Bodies.size());
	m_Distances.resize(m_Bodies.size()*m_Bodies.size());

//	m_ProjectilePool.resize(20 * m_Bodies.size()); // 20 shots are allowed per object at maximum

	// load meshes
		// get the unique keys from the map
	std::vector<std::string> mesh_types;
	for (auto const& element : parsed_input.mesh_files)
		mesh_types.push_back(element.first);

		// load a mesh for each unique key from the map

	std::map<std::string, int> mesh_type_id_lookup; // A helper objects to connect mesh names with mesh ID-s
	m_Meshes.resize(mesh_types.size());
	for (int i = 0; i < mesh_types.size(); i++)
	{
		m_Meshes[i] = std::move(TexturedShadedMesh(parsed_input.mesh_files[mesh_types[i]], parsed_input.mesh_textures[mesh_types[i]]));
		mesh_type_id_lookup[mesh_types[i]] = i;
	}

	m_MeshIndices.resize(bodyTypes.size());
	for (int i = 0; i < bodyTypes.size(); i++)
		m_MeshIndices[i] = mesh_type_id_lookup[bodyTypes[i]];

	// Create shader and set its uniforms
	m_TextureShader = std::move(Shader(ParseShader("src/renderer/shader_sources/vertex_shader_textured_shaded.glsl"), ParseShader("src/renderer/shader_sources/fragment_shader_textured_shaded.glsl")));
	{
		m_TextureShader.Bind();
		m_TextureShader.UploadUniformFloat3("body_translation", glm::vec3(0.0f, 0.0f, 0.0f));
		m_TextureShader.UploadUniformMat3("body_orientation", glm::mat3(1.0f));
		m_TextureShader.UploadUniformFloat("body_scale", 1.0f);
		m_TextureShader.UploadUniformFloat3("scene_translation", glm::vec3(0.0f, 0.0f, 0.0f));
		m_TextureShader.UploadUniformFloat3("observer_translation", glm::vec3(0.0f, 0.0f, 0.0f));
		m_TextureShader.UploadUniformMat3("observer_orientation", glm::mat3(1.0f));
		m_TextureShader.UploadUniformFloat("zoom_level", 1.0f);
		m_TextureShader.UploadUniformFloat("aspect_ratio", 1.0f); // if needed, the aspect ratio will be changeable somewhere else

		int samplers[32];
		for (uint32_t i = 0; i < 32; i++) { samplers[i] = i; }
		m_TextureShader.UploadUniformIntArray("u_Textures", samplers, 32);
	}

}

Scene::~Scene()
{
	m_ProjectilePool.~ProjectilePool();
	m_TextureShader.~Shader();
	m_Skybox.~Skybox();

	for (int i = 0; i < m_Meshes.size(); i++)
		m_Meshes[i].~TexturedShadedMesh();
}

void Scene::Update(float deltaTime)
{
	for (int i = 0; i < m_Bodies.size(); i++)
	{
		m_Bodies[i].location += deltaTime * m_Bodies[i].velocity;
		m_Bodies[i].orientation = Rotation(deltaTime*m_Bodies[i].angularVelocity.length(), m_Bodies[i].angularVelocity) * m_Bodies[i].orientation;
	}

	m_ProjectilePool.Update(deltaTime);
}

void Scene::Update(float deltaTime, AccelerationFunction accelerationFunc)
{
	//	std::vector<Vec3D> accelerations = accelerationFunc(m_Bodies);
	accelerationFunc(m_Bodies, m_Accelerations, m_AngularAccelerations);
	for (int i = 0; i < m_Bodies.size(); i++)
	{
		m_Bodies[i].location += deltaTime * m_Bodies[i].velocity;
		m_Bodies[i].velocity += deltaTime * m_Accelerations[i];
		m_Bodies[i].orientation = Rotation(deltaTime*m_Bodies[i].angularVelocity.length(), m_Bodies[i].angularVelocity) * m_Bodies[i].orientation;
		m_Bodies[i].angularVelocity += deltaTime * m_AngularAccelerations[i];
	}

	m_ProjectilePool.Update(deltaTime);
}

void Scene::UpdateWithCollision(float deltaTime, AccelerationFunction accelerationFunc)
{
	//	std::vector<Vec3D> accelerations = accelerationFunc(m_Bodies);
	accelerationFunc(m_Bodies, m_Accelerations, m_AngularAccelerations);
	CalcMinDistances(deltaTime);

	static Vec3D v_eff;
	static Vec3D tempV;

	for (int i = 0; i < m_Bodies.size(); i++)
	{
		for (int j = i + 1; j < m_Bodies.size(); j++)
		{
			// do this only for active bodies
			if (m_Integrities[i] > 0.0f && m_Integrities[j] > 0.0f)
			{
				// swap momentums in the center off mass inertial frame at collision
				if (m_Distances[i*m_Bodies.size() + j] < m_Bodies[i].scale + m_Bodies[j].scale)
				{
					Vec3D v_i0 = m_Bodies[i].velocity;
					Vec3D v_j0 = m_Bodies[j].velocity;

					v_eff = (m_Bodies[i].mass*m_Bodies[i].velocity + m_Bodies[j].mass*m_Bodies[j].velocity) / (m_Bodies[i].mass + m_Bodies[j].mass);
					tempV = m_Bodies[i].velocity - v_eff;
					m_Bodies[i].velocity = v_eff + (m_Bodies[j].mass / m_Bodies[i].mass) * (m_Bodies[j].velocity - v_eff);
					m_Bodies[j].velocity = v_eff + (m_Bodies[i].mass / m_Bodies[j].mass) * tempV;

					// Make the change in integrity proportional to the transferred momentum
					m_Integrities[i] -= 0.1f * m_Bodies[i].mass * (v_i0- m_Bodies[i].velocity).length(); // 0.1f should be a PARAMETER called something like collision sensitivity
					m_Integrities[j] -= 0.1f * m_Bodies[j].mass * (v_j0 - m_Bodies[j].velocity).length();
				}
			}
		}
	}

//	static float boundary = 5000000.0f;
	for (int i = 0; i < m_Bodies.size(); i++)
	{
		// update only active bodies
		if (m_Integrities[i] > 0.0f)
		{
			m_Bodies[i].location += deltaTime * m_Bodies[i].velocity;
			// implement periodic boudaries
	//		{
	//			m_Bodies[i].location.x += m_Bodies[i].location.x < (-1.0f*boundary) ? boundary : 0.0f;
	//			m_Bodies[i].location.x -= m_Bodies[i].location.x > boundary ? boundary : 0.0f;
	//			m_Bodies[i].location.y += m_Bodies[i].location.y < (-1.0f*boundary) ? boundary : 0.0f;
	//			m_Bodies[i].location.y -= m_Bodies[i].location.y > boundary ? boundary : 0.0f;
	//			m_Bodies[i].location.z += m_Bodies[i].location.z < (-1.0f*boundary) ? boundary : 0.0f;
	//			m_Bodies[i].location.z -= m_Bodies[i].location.z > boundary ? boundary : 0.0f;
	//		}
			m_Bodies[i].velocity += deltaTime * m_Accelerations[i];
			m_Bodies[i].orientation = Rotation(deltaTime*m_Bodies[i].angularVelocity.length(), m_Bodies[i].angularVelocity) * m_Bodies[i].orientation;
			m_Bodies[i].angularVelocity += deltaTime * m_AngularAccelerations[i];
		}
	}

	m_ProjectilePool.Update(deltaTime);
}

void Scene::OnShoot(Body* ownerBodyPtr, float ownerRange)
{
	// Apparently these addresses have to be converted to integers
	int bodyIndex = ((int)ownerBodyPtr - (int)&m_Bodies[0])/sizeof(Body);
	// Or just take the plain difference of the addresses
//	int bodyIndex = (ownerBodyPtr - &m_Bodies[0]);
	int hitTarget = m_ProjectilePool.Emit(bodyIndex, ownerRange, m_Bodies, m_Integrities);

	if (hitTarget >= 0)
	{
		OnHit(hitTarget, 1.0f, ownerBodyPtr); // hitStrength PARAMETER
	}
}



void Scene::Draw(Observer obs)
{
	// Render
	glClearColor(0.0f, 0.05f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_Skybox.Draw(obs);

	m_TextureShader.Bind();
	obs.SetObserverInShader(m_TextureShader);
	for (int i = 0; i < m_Bodies.size(); i++)
	{
		// Draw only active bodies
		if (m_Integrities[i] > 0.0f)
		{
			m_TextureShader.UploadUniformFloat3("body_translation", m_Bodies[i].location.Glm());
			m_TextureShader.UploadUniformMat3("body_orientation", m_Bodies[i].orientation.Glm());
			m_TextureShader.UploadUniformFloat("body_scale", m_Bodies[i].scale);
			m_Meshes[m_MeshIndices[i]].Draw();
		}
	}

}

void Scene::Draw(Player& player)
{
	// Render
	glClearColor(0.0f, 0.05f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_Skybox.Draw(player.m_Observer);

	m_TextureShader.Bind();
	player.m_Observer.SetObserverInShader(m_TextureShader);
	for (int i = 0; i < m_Bodies.size(); i++)
	{
		// Draw only active bodies
		if (m_Integrities[i] > 0.0f)
		{
			m_TextureShader.UploadUniformFloat3("body_translation", m_Bodies[i].location.Glm());
			m_TextureShader.UploadUniformMat3("body_orientation", m_Bodies[i].orientation.Glm());
			m_TextureShader.UploadUniformFloat("body_scale", m_Bodies[i].scale);
			if (player.m_BodyPtr != &m_Bodies[i]) { m_Meshes[m_MeshIndices[i]].Draw(); }
		}
	}

	m_ProjectilePool.Draw(player);
}

void Scene::SetAspectRatio(float aspectRatio)
{
	m_TextureShader.Bind();
	m_TextureShader.UploadUniformFloat("aspect_ratio", aspectRatio);
	m_Skybox.SetShaderAspectRatio(aspectRatio);
	m_ProjectilePool.SetAspectRatio(aspectRatio);
}

void Scene::SetSceneTranslation(glm::vec3 scene_translation)
{
	m_TextureShader.UploadUniformFloat3("scene_translation", scene_translation);
}

Body* Scene::GetBodyPtr(int i)
{
	if (i < 0)
	{
		while (i < 0)
		{
			i += m_Bodies.size();
		}
	}
	while (i >= m_Bodies.size())
	{
		i -= m_Bodies.size();
	}

	return &m_Bodies[i];
}

void Scene::CalcMinDistances(float deltaTime, float dvThreshold)
{
	int n = m_Bodies.size();
	float lambda = 0.0f;
	Vec3D dr = { 0,0,0 }, dv = { 0,0,0 };
	for (int i = 0; i < n; i++)
	{
		for (int j = i+1; j < n; j++)
		{
			dr = m_Bodies[i].location - m_Bodies[j].location;
			dv = m_Bodies[i].velocity - m_Bodies[j].velocity;
//			lambda = -(dr*dv) / dv.lengthSquare();
			lambda = dv.lengthSquare() < dvThreshold ? 0 : -(dr*dv) / dv.lengthSquare();
			m_Distances[i*n + j] = (dr + dv * deltaTime*lambda).length();
			m_Distances[j*n + i] = lambda; // store the lambda values in the unused half of the distance matrix
		}
	}
}

//void Scene::OnHit(int bodyIndex, Projectile& projectile, float hitStrength)
void Scene::OnHit(int bodyIndex, float hitStrength, Body* shooterBody)
{
	m_Integrities[bodyIndex] -= hitStrength;
	m_Bodies[bodyIndex].angularVelocity += 0.1f * (hitStrength / m_Bodies[bodyIndex].mass) * shooterBody->orientation.f1; // PARAMETER angular momentum transfer OnHit
	m_Bodies[bodyIndex].velocity += (hitStrength / m_Bodies[bodyIndex].mass) * shooterBody->orientation.f3;
}







