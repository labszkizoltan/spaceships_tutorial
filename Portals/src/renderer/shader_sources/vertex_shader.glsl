
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

uniform vec3 body_translation;
uniform mat3 body_orientation;
uniform float body_scale;

uniform vec3 observer_translation;
uniform mat3 observer_orientation;

uniform float zoom_level;

out vec3 outColor;

void main()
{
	vec3 e1 = observer_orientation * body_orientation[0];
	vec3 e2 = observer_orientation * body_orientation[1];
	vec3 e3 = observer_orientation * body_orientation[2];

	vec3 position_tmp = (body_translation-observer_translation) + body_scale*(aPos[0] * e1 + aPos[1] * e2 + aPos[2] * e3);
	float r = length(position_tmp);
	float phi = atan(position_tmp.x, position_tmp.y);
	float theta = acos(position_tmp.z / r);

	float new_r = zoom_level * theta;
	gl_Position = vec4(vec3(new_r*cos(phi), new_r*sin(phi), max(0, atan(r) / 1.570787f)), 1.0f);

	outColor = aColor;
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
