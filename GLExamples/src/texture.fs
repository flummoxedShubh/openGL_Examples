#version 330 core
in vec2 TexCoord;
in vec3 Color;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	//linearly interpolate between 2 textures 60% of tex1 and 40% of tex2
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.4) *  vec4(Color, 1.0f);
}