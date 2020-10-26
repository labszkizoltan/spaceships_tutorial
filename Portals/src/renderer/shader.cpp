
#include "renderer.h"
#include "shader.h"

#include "glm/gtc/type_ptr.hpp"



// ---------------------- //
// ----- UniformSet ----- //
// ---------------------- //


// m_RendererID identifies the shader
void UniformSet::AddUniform(const std::string& name, uint32_t m_RendererID)
{
	uniformLocations[name] = glGetUniformLocation(m_RendererID, name.c_str());
}

bool UniformSet::Contains(const std::string& name)
{
	// the uniform with the given name does not exist in the unordered_map
	if (uniformLocations.find(name) == uniformLocations.end()) { return false; }
	return true;
}


// ------------------ //
// ----- Shader ----- //
// ------------------ //


std::string ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);
	std::stringstream ss;

	std::string line;
	while (getline(stream, line))
	{
		ss << line << '\n';
	}

	return ss.str();
}

static uint32_t CompileShader(uint32_t type, const std::string& source)
{
	uint32_t id = glCreateShader(type);
	// source should exist at this point
	const char* src = source.c_str(); // equivalent to &source[0]
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	// TODO: Error handling
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));

		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader\n";
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

// This function is basically not needed, since the shader constructor does the same
static uint32_t CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
	uint32_t program = glCreateProgram();
	uint32_t vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}


// Use like this: Shader my_shader(ParseShader("folder/folder2/vertex_src.glsl"), ParseShader("folder/folder2/fragment_src.glsl"));
Shader::Shader(const std::string & vertexSrc, const std::string & fragmentSrc)
{
	m_RendererID = glCreateProgram();
	uint32_t vs = CompileShader(GL_VERTEX_SHADER, vertexSrc);
	uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);

	GLCall(glAttachShader(m_RendererID, vs));
	GLCall(glAttachShader(m_RendererID, fs));
	GLCall(glLinkProgram(m_RendererID));
	GLCall(glValidateProgram(m_RendererID));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));
}

Shader::~Shader()
{
//	glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
	glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::UploadUniformInt(const std::string & name, const int value)
{
	if (m_UniformLocations.Contains(name))
	{
		glUniform1i(GetUniformLocation(name), value);
	}
	else
	{
		std::cout << "Shader::UploadUniformInt() : uniform called \"" << name << "\" does not exist in shader , or hasn't been added with Shader::SearchAndAddUniform()";
	}
}

void Shader::UploadUniformIntArray(const std::string & name, int * values, uint32_t count)
{
	if (m_UniformLocations.Contains(name))
	{
		glUniform1iv(GetUniformLocation(name), count, values);
	}
	else
	{
		std::cout << "Shader::UploadUniformIntArray() : uniform called \"" << name << "\" does not exist in shader , or hasn't been added with Shader::SearchAndAddUniform()";
	}
}

void Shader::UploadUniformFloat(const std::string & name, const float value)
{
	if (m_UniformLocations.Contains(name))
	{
		glUniform1f(GetUniformLocation(name), value);
	}
	else
	{
		std::cout << "Shader::UploadUniformFloat() : uniform called \"" << name << "\" does not exist in shader, or hasn't been added with Shader::SearchAndAddUniform()";
	}
}


void Shader::UploadUniformFloat2(const std::string & name, const glm::vec2 & value)
{
	if (m_UniformLocations.Contains(name))
	{
		glUniform2f(GetUniformLocation(name), value.x, value.y);
	}
	else
	{
		std::cout << "Shader::UploadUniformFloat2() : uniform called \"" << name << "\" does not exist in shader, or hasn't been added with Shader::SearchAndAddUniform()";
	}
}

void Shader::UploadUniformFloat3(const std::string & name, const glm::vec3 & value)
{
	if (m_UniformLocations.Contains(name))
	{
		glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
	}
	else
	{
		std::cout << "Shader::UploadUniformFloat3() : uniform called \"" << name << "\" does not exist in shader, or hasn't been added with Shader::SearchAndAddUniform()";
	}
}

void Shader::UploadUniformFloat4(const std::string & name, const glm::vec4 & value)
{
	if (m_UniformLocations.Contains(name))
	{
		glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
	}
	else
	{
		std::cout << "Shader::UploadUniformFloat4() : uniform called \"" << name << "\" does not exist in shader, or hasn't been added with Shader::SearchAndAddUniform()";
	}
}

void Shader::UploadUniformMat3(const std::string & name, const glm::mat3 & value)
{
	if (m_UniformLocations.Contains(name))
	{
		glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	}
	else
	{
		std::cout << "Shader::UploadUniformMat3() : uniform called \"" << name << "\" does not exist in shader, or hasn't been added with Shader::SearchAndAddUniform()";
	}
}

void Shader::UploadUniformMat4(const std::string & name, const glm::mat4 & value)
{
	if (m_UniformLocations.Contains(name))
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	}
	else
	{
		std::cout << "Shader::UploadUniformMat4() : uniform called \"" << name << "\" does not exist in shader, or hasn't been added with Shader::SearchAndAddUniform()";
	}
}
