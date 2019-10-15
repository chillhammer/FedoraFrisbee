#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 texCoord;


uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

// Draws in shade of red with transparency
void main()
{
	gl_Position = u_ViewProjection * u_Model * vec4(position, 1.0);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

void main()
{
	color = vec4(0.5f, 0.1f, 0.1f, 0.9f);
};