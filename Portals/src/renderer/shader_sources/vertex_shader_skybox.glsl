
#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in float aTexID;

// uniform mat3 observer_orientation;
// uniform float zoom_level;
uniform float obs_param[13];

uniform float aspect_ratio;


out vec2 vTexCoord;
out float vTexID;

void main()
{
//	vec3 observer_translation = vec3(obs_param[0], obs_param[1], obs_param[2]); // in skybox, the translation of the observer is irrelevant
	mat3 observer_orientation = mat3(obs_param[3], obs_param[4], obs_param[5], obs_param[6], obs_param[7], obs_param[8], obs_param[9], obs_param[10], obs_param[11]);
	float zoom_level = obs_param[12];

	vec3 position_tmp = vec3(
		dot(aPos, observer_orientation[0]),
		dot(aPos, observer_orientation[1]),
		dot(aPos, observer_orientation[2])
	);

	float r = length(position_tmp);
	float theta = acos(position_tmp.z / r);
	float rho = length(vec2(position_tmp.x, position_tmp.y));

//	float aspect_ratio = 1.777; // aspect ratio of a 1280 x 720 screen
	float theta_max = 1.0471955; // 60 degrees
	float r_min = 0.01;

	float new_r = zoom_level * theta / theta_max;
	float z_sign = position_tmp[2] / abs(position_tmp[2]);

	gl_Position = vec4(
		new_r*position_tmp.x / (rho*aspect_ratio),
		new_r*position_tmp.y / rho,
		2.0 * atan(z_sign*r - r_min) / 1.570787f - 1.0,
		1.0f
	);

	vTexCoord = aTexCoord;
	vTexID = aTexID;
}








