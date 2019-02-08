#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

// Shader without normals or lighting
void main()
{
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec3 u_Color;
uniform sampler2D u_Texture;
uniform bool u_UseTexture;

void main()
{
	if (u_UseTexture)
		color = texture(u_Texture, v_TexCoord);
	else
		color = vec4(u_Color, 1.0);
};