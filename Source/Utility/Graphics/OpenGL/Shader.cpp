#include <FedPCH.h>
#include "Shader.h"
#include <fstream>

#include <GL/glew.h>

namespace Fed
{
	ShaderProgramSource Shader::ParseShader(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		enum class ShaderType {
			NONE = -1, VERTEX = 0, FRAGMENT = 1
		};
		std::string line;
		std::stringstream ss[2];
		ShaderType type = ShaderType::NONE;
		while (getline(stream, line))
		{
			if (line.find("#shader") != std::string::npos)
			{

				if (line.find("vertex") != std::string::npos)
				{
					type = ShaderType::VERTEX;
				}
				else if (line.find("fragment") != std::string::npos)
				{
					type = ShaderType::FRAGMENT;
				}
			}
			else
			{
#ifdef __EMSCRIPTEN__
				// WebGL2 wants GLSL ES 3.00. The on-disk shaders are written for
				// desktop OpenGL 3.3 (#version 330 core). Rewrite the version line
				// and emit default precision qualifiers; the rest of GLSL 330's
				// modern syntax (layout(location=...), out, texture()) carries over.
				if (line.find("#version") != std::string::npos &&
				    line.find("330") != std::string::npos)
				{
					ss[(int)type] << "#version 300 es\n";
					ss[(int)type] << "precision highp float;\n";
					ss[(int)type] << "precision highp int;\n";
					ss[(int)type] << "precision highp sampler2D;\n";
					ss[(int)type] << "precision highp samplerCube;\n";
					continue;
				}
#endif
				//Adding to appropriate string stream
				ss[(int)type] << line << '\n';
			}
		}
		return { ss[0].str(), ss[1].str() };
	}

	unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
	{
		unsigned int id = glCreateShader(type);
		const char* src = source.c_str();
		GLCall(glShaderSource(id, 1, &src, nullptr));
		GLCall(glCompileShader(id));
		// Error handling
		int result;
		GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
		if (result == GL_FALSE)
		{
			int length;
			GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
			char* message = (char*)alloca(length * sizeof(char));
			GLCall(glGetShaderInfoLog(id, length, &length, message));
			LOG_ERROR("Failed to compile {0} shader!",
				(type == GL_VERTEX_SHADER ? "vertex" : "fragment"));
			if (length > 1)
				LOG_ERROR("{0}", message);
			GLCall(glDeleteShader(id));
			return 0;
		}
		return id;
	}

	// Compile both vertex and fragment shaders. Returns a program
	unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
	{
		unsigned int program = glCreateProgram();
		unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
		unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
		if (vs == 0 || fs == 0)
		{
			LOG_ERROR("Failed to create shader program: {0}", m_FilePath);
			if (vs != 0)
				GLCall(glDeleteShader(vs));
			if (fs != 0)
				GLCall(glDeleteShader(fs));
			GLCall(glDeleteProgram(program));
			return 0;
		}

		GLCall(glAttachShader(program, vs));
		GLCall(glAttachShader(program, fs));
		GLCall(glLinkProgram(program));
		GLCall(glValidateProgram(program));
		GLCall(glDeleteShader(vs));
		GLCall(glDeleteShader(fs));
		return program;
	}

	Shader::Shader(const std::string & filepath) :
		m_FilePath(filepath), m_RendererID(0)
	{
		ShaderProgramSource source = ParseShader(filepath);
		m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
	}

	Shader::~Shader()
	{
		GLCall(glDeleteProgram(m_RendererID));
	}


	void Shader::Bind() const
	{
		GLCall(glUseProgram(m_RendererID));
	}

	void Shader::Unbind() const
	{
		GLCall(glUseProgram(0));
	}

	void Shader::SetUniform4f(const std::string & name, float v0, float v1, float v2, float v3)
	{
		GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
	}

	void Shader::SetUniform3f(const std::string & name, float v0, float v1, float v2)
	{
		// Removed Debug Check because it kept acting up here
		glUniform3f(GetUniformLocation(name), v0, v1, v2);
	}

	void Shader::SetUniform1f(const std::string & name, float v0)
	{
		GLCall(glUniform1f(GetUniformLocation(name), v0));
	}
	void Shader::SetUniform1i(const std::string & name, int v0)
	{
		GLCall(glUniform1i(GetUniformLocation(name), v0));
	}

	void Shader::SetUniformMat4f(const std::string& name, const glm::mat4 & matrix)
	{
		GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
	}

	int Shader::GetUniformLocation(const std::string & name)
	{
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache[name];
		GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
		m_UniformLocationCache[name] = location;
		return location;
	}
}
