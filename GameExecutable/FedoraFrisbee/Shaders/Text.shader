#shader vertex
#version 330 core

layout(location = 0) in vec4 vertex; // [2: position, 2: texcoords]

out vec2 v_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_Projection;

void main()
{
	v_TexCoord = vertex.zw;
	gl_Position = u_Projection * u_Model * vec4(vertex.xy, 0.0, 1.0);
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
	color = vec4(u_Color.x, u_Color.y, u_Color.z, texture(u_Texture, v_TexCoord).r);
};