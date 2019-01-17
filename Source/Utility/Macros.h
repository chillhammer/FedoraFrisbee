#pragma once
#include <Logger/Log.h>
#include <vec3.hpp>
#include <glm.hpp>
#include <gtc/quaternion.hpp>
#include <GL/glew.h>

#ifdef ENABLE_ASSERTS
	#define ASSERT(x, ...) {if (!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
	#define ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define GLM_ENABLE_EXPERIMENTAL

typedef glm::vec3 Vector3;
typedef glm::vec2 Vector2;
typedef glm::mat4 Matrix4x4;
typedef glm::quat Quaternion;
typedef glm::vec4 Vector4;

#pragma region Debug OpenGl Call
#define GLCall(x)	\
	GLClearError();	\
	x;				\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__), "OpenGL Error!")

static void GLClearError()
{
	while (glGetError() != 0);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		LOG_ERROR("[OpenGL Error] ({0}): {1}\n{2}: {3}", error, function, file, line);
		return false;
	}
	return true;
}
#pragma endregion
