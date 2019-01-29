#pragma once
#include <string>
#include <unordered_map>
#include <glm.hpp>

namespace Fed
{
	struct ShaderProgramSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	/**
		OpenGL wrapper for a shader. 
		Allows to set uniforms and parse from text easily.
	**/
	class Shader
	{
	private:
		unsigned int m_RendererID;
		std::string m_FilePath;
		std::unordered_map<std::string, int> m_UniformLocationCache;

	public:
		Shader(const std::string& filepath);
		virtual ~Shader();

		void Bind() const;
		void Unbind() const;

		// Set Uniforms
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void SetUniform3f(const std::string& name, float v0, float v1, float v2);
		void SetUniform1f(const std::string& name, float v0);
		void SetUniform1i(const std::string& name, int v0);
		void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
	private:
		int GetUniformLocation(const std::string& name);
		ShaderProgramSource ParseShader(const std::string& filepath);
		unsigned int CompileShader(unsigned int type, const std::string& source);
		unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	};
}
