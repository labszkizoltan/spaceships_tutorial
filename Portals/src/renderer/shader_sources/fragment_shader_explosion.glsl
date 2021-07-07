

#version 450 core

in vec3 outColor;
uniform float alpha;
out vec4 FragColor;

void main()
{
	FragColor = vec4(outColor, alpha);
}

