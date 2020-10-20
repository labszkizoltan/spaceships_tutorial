#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>


//#include <glad/glad.h>
#include "vendor/glm/glm/glm.hpp"

typedef int GLint;

struct UniformSet
{
	std::unordered_map<std::string, GLint> uniformLocations;

	// m_RendererID identifies the shader
	void AddUniform(const std::string& name, uint32_t m_RendererID);
	bool Contains(const std::string& name);
	operator std::unordered_map<std::string, GLint>() const { return uniformLocations; }


};



std::string ParseShader(const std::string& filepath);


class Shader
{
public:
//	Shader(const std::string& filepath);
	Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void UploadUniformInt(const std::string& name, const int value);
	void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);
	void UploadUniformFloat(const std::string& name, const float value);
	void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
	void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
	void UploadUniformFloat4(const std::string& name, const glm::vec4& value);
	void UploadUniformMat3(const std::string& name, const glm::mat3& value);
	void UploadUniformMat4(const std::string& name, const glm::mat4& value);

	const std::string GetName() const  { return m_Name; };
	
	void SearchAndAddUniform(const std::string& name) { m_UniformLocations.AddUniform(name, m_RendererID); }

private:
	GLint GetUniformLocation(const std::string& name) { return m_UniformLocations.uniformLocations[name]; }

private:
	uint32_t m_RendererID;
	std::string m_Name;
	UniformSet m_UniformLocations;
};
















