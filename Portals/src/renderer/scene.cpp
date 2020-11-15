
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
		if (!line.empty()) { file_in_memory.push_back(line); } // leave out empty lines
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
	for (int i = 0; i < parsed_input.body_types.size(); i++)
	{
		bodyTypes[i] = parsed_input.body_types[i];
		m_Bodies[i] = parsed_input.bodies[i];
	}
	m_Accelerations.resize(m_Bodies.size());
	m_Distances.resize(m_Bodies.size()*m_Bodies.size());

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


}

void Scene::Update(float deltaTime)
{
	for (int i = 0; i < m_Bodies.size(); i++)
	{
		m_Bodies[i].location += deltaTime * m_Bodies[i].velocity;
		m_Bodies[i].orientation = Rotation(deltaTime*m_Bodies[i].angularVelocity.length(), m_Bodies[i].angularVelocity) * m_Bodies[i].orientation;
	}
}

void Scene::Update(float deltaTime, AccelerationFunction accelerationFunc)
{
	//	std::vector<Vec3D> accelerations = accelerationFunc(m_Bodies);
	accelerationFunc(m_Bodies, m_Accelerations);
	for (int i = 0; i < m_Bodies.size(); i++)
	{
		m_Bodies[i].location += deltaTime * m_Bodies[i].velocity;
		m_Bodies[i].velocity += deltaTime * m_Accelerations[i];
		m_Bodies[i].orientation = Rotation(deltaTime*m_Bodies[i].angularVelocity.length(), m_Bodies[i].angularVelocity) * m_Bodies[i].orientation;
	}
}

void Scene::UpdateWithCollision(float deltaTime, AccelerationFunction accelerationFunc)
{
	//	std::vector<Vec3D> accelerations = accelerationFunc(m_Bodies);
	accelerationFunc(m_Bodies, m_Accelerations);
	CalcMinDistances(deltaTime);
	for (int i = 0; i < m_Bodies.size(); i++)
	{
		for (int j = i + 1; j < m_Bodies.size(); j++)
		{
			// for now, swap velocities when two bodies collide
			if (m_Distances[i*m_Bodies.size() + j] < m_Bodies[i].scale + m_Bodies[j].scale)
			{
				Vec3D tempV = m_Bodies[i].velocity;
				m_Bodies[i].velocity = m_Bodies[j].velocity;
				m_Bodies[j].velocity = tempV;
			}
		}
	}

	static float boundary = 400.0f;
	for (int i = 0; i < m_Bodies.size(); i++)
	{
		m_Bodies[i].location += deltaTime * m_Bodies[i].velocity;
		// implement periodic boudaries
		{
			m_Bodies[i].location.x += m_Bodies[i].location.x < (-1.0f*boundary) ? 1000.0f : 0.0f;
			m_Bodies[i].location.x -= m_Bodies[i].location.x > boundary ? 1000.0f : 0.0f;
			m_Bodies[i].location.y += m_Bodies[i].location.y < (-1.0f*boundary) ? 1000.0f : 0.0f;
			m_Bodies[i].location.y -= m_Bodies[i].location.y > boundary ? 1000.0f : 0.0f;
			m_Bodies[i].location.z += m_Bodies[i].location.z < (-1.0f*boundary) ? 1000.0f : 0.0f;
			m_Bodies[i].location.z -= m_Bodies[i].location.z > boundary ? 1000.0f : 0.0f;
		}
		m_Bodies[i].velocity += deltaTime * m_Accelerations[i];
		m_Bodies[i].orientation = Rotation(deltaTime*m_Bodies[i].angularVelocity.length(), m_Bodies[i].angularVelocity) * m_Bodies[i].orientation;
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
		m_TextureShader.UploadUniformFloat3("body_translation", m_Bodies[i].location.Glm());
		m_TextureShader.UploadUniformMat3("body_orientation", m_Bodies[i].orientation.Glm());
		m_TextureShader.UploadUniformFloat("body_scale", m_Bodies[i].scale);
		m_Meshes[m_MeshIndices[i]].Draw();
	}
}

void Scene::SetAspectRatio(float aspectRatio)
{
	m_TextureShader.Bind();
	m_TextureShader.UploadUniformFloat("aspect_ratio", aspectRatio);
	m_Skybox.SetShaderAspectRatio(aspectRatio);
}

void Scene::SetSceneTranslation(glm::vec3 scene_translation)
{
	m_TextureShader.UploadUniformFloat3("scene_translation", scene_translation);
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
			m_Distances[i*n + j] = (dr+dv*deltaTime*lambda).length();
		}
	}
}







