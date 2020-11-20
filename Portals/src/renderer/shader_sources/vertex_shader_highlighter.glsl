
#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

uniform vec3 body_translation;

uniform float obs_param[13];

uniform float aspect_ratio;

out vec4 vColor;

void main()
{
	vec3 observer_translation = vec3(obs_param[0], obs_param[1], obs_param[2]);
	mat3 observer_orientation = mat3(obs_param[3], obs_param[4], obs_param[5], obs_param[6], obs_param[7], obs_param[8], obs_param[9], obs_param[10], obs_param[11]);
	float zoom_level = obs_param[12];

	vec3 position_tmp = body_translation - observer_translation;
	position_tmp = vec3(
		dot(position_tmp, observer_orientation[0]),
		dot(position_tmp, observer_orientation[1]),
		dot(position_tmp, observer_orientation[2])
	);

	float r = length(position_tmp);
	float theta = acos(position_tmp.z / r);
	float rho = length(vec2(position_tmp.x, position_tmp.y));

	float theta_max = 1.0471955; // 60 degrees
	float r_max = 5000000.0f;
	float r_min = 0.25;

	float new_r = zoom_level * theta / theta_max;

	vec4 centerPos = vec4(
		new_r*position_tmp.x / (rho*aspect_ratio),
		new_r*position_tmp.y / rho,
		2.0 * (sign(position_tmp[2])*r - r_min) / r_max - 1, // finite sight range, ending at distance == r_max
//		2.0 * atan(sign(position_tmp[2])*r - r_min) / 1.570787f - 1.0, // infinite sight range
		1.0f
);

	float yDisplacement = -0.005;
	gl_Position = vec4(aPos.x/aspect_ratio, aPos.y+yDisplacement, aPos.z, 0)+centerPos;
	vColor = vec4(aColor,1);
}

