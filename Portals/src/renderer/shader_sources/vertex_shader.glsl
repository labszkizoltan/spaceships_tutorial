
// OpenGL clip space was left handed always!!!
// https://stackoverflow.com/questions/17650219/when-is-z-axis-flipped-opengl


#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

uniform vec3 body_translation;
uniform mat3 body_orientation;
uniform float body_scale;

uniform vec3 observer_translation;
uniform mat3 observer_orientation;

uniform float zoom_level;

uniform float aspect_ratio;

out vec3 outColor;

void main()
{
	vec3 position_tmp = body_translation-observer_translation + body_scale*(aPos[0] * body_orientation[0] + aPos[1] * body_orientation[1] + aPos[2] * body_orientation[2] );
	position_tmp = vec3(
		dot(position_tmp, observer_orientation[0]),
		dot(position_tmp, observer_orientation[1]),
		dot(position_tmp, observer_orientation[2])
	);

	float r = length(position_tmp);
	float theta = acos(position_tmp.z / r);
	float rho = length(vec2(position_tmp.x, position_tmp.y));

	float theta_max = 1.0471955; // 60 degrees
//	float r_max = 200.0f;
	float r_min = 0.25;

	float new_r = zoom_level * theta/theta_max;

	gl_Position = vec4(
		new_r*position_tmp.x/(rho*aspect_ratio),
		new_r*position_tmp.y/rho,
//		2*(sign(position_tmp[2])*r-r_min)/r_max-1, // finite sight range, ending at distance == r_max
		2.0 * atan(sign(position_tmp[2])*r - r_min) / 1.570787f - 1.0, // infinite sight range
		1.0f
	);

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
