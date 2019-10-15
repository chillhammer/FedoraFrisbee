#shader vertex
#version 330 core

layout(location = 0) in vec3 position;

out vec3 v_TexCoord;
uniform mat4 u_ViewProjection;

void main()
{
	v_TexCoord = position;
	vec4 pos = u_ViewProjection * vec4(position * 100.0f, 1.0);
	gl_Position = pos.xyww;
};

#shader fragment
#version 330 core
out vec4 color;

in vec3 v_TexCoord;

uniform samplerCube u_Skybox;

void main()
{
	color = texture(u_Skybox, v_TexCoord);
}
