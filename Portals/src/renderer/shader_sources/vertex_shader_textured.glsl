
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float aID;

uniform vec3 translation[3];

out vec3 outColor;
out vec2 v_TexCoord;

void main()
{
	//	vec3 translation[2] = vec3[](vec3(-0.2, 0.0, 0.2), vec3(0.2, 0.0, 0.0));

	int index = int(aID);

	mat3 rotation = mat3(
		vec3(0.866, -0.5, 0.0), // first column
		vec3(0.5, 0.866, 0.0), // second column
		vec3(0.1, 0.0, 0.1)  // third column
	);

	//	gl_Position = vec4(rotation * aPos + translation[index], 1.0f);

	vec3 position_tmp = rotation * aPos + translation[index];
	float r = length(position_tmp);
	float phi = atan(position_tmp.x, position_tmp.y);
	float theta = acos(position_tmp.z / r);

	float new_r = 1.8 * theta;
	gl_Position = vec4(vec3(new_r*cos(phi), new_r*sin(phi), max(0, atan(r) / 1.570787f)), 1.0f);

	outColor = aColor;

	v_TexCoord = texCoord;
}


// description of matrix constructors: https://www.khronos.org/opengl/wiki/Data_Type_(GLSL)
// 
// mat2(
// 	float, float,   // first column
// 	float, float);  // second column
// 
// mat4(
// 	vec4,           // first column
// 	vec4,           // second column
// 	vec4,           // third column
// 	vec4);          // fourth column
// 
// mat3(
// 	vec2, float,    // first column
// 	vec2, float,    // second column
// 	vec2, float);   // third column
// 
s