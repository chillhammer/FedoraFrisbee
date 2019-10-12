#shader vertex
#version 330 core

layout(location = 0) in vec4 vertex; // [2: position, 2: texcoords]

out vec2 v_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

void main()
{
	v_TexCoord = vertex.zw;
	gl_Position = u_ViewProjection * u_Model * vec4(vertex.xy, 0.0, 1.0);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;
uniform bool u_UseTexture;
uniform vec3 u_Color;

void main()
{
	if (u_UseTexture)
		color = texture(u_Texture, v_TexCoord);
	else
		color = vec4(u_Color, 1.0);

	color = vec4(1.0, 1.0, 1.0, texture(u_Texture, v_TexCoord).r);
	//color = vec4(1.0, 1.0, 1.0, 0.5f);
	//color = vec4(1.0, 1.0, 1.0, 1.0);
};