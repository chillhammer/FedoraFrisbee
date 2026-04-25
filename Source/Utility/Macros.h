#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <cmath>
#include <Logger/Log.h>
#include <vec3.hpp>
#include <glm.hpp>
#include <gtx/norm.hpp>
#include <gtc/quaternion.hpp>
#include <GL/glew.h>

#ifdef ENABLE_ASSERTS
	#define ASSERT(x, ...) {if (!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
	#define ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

typedef glm::vec3 Vector3;
typedef glm::vec2 Vector2;
typedef glm::mat4 Matrix4x4;
typedef glm::quat Quaternion;
typedef glm::vec4 Vector4;

inline bool IsFiniteVector(const Vector2& value)
{
	return std::isfinite(value.x) && std::isfinite(value.y);
}

inline bool IsFiniteVector(const Vector3& value)
{
	return std::isfinite(value.x) && std::isfinite(value.y) && std::isfinite(value.z);
}

inline bool IsFiniteVector(const Vector4& value)
{
	return std::isfinite(value.x) && std::isfinite(value.y) && std::isfinite(value.z) && std::isfinite(value.w);
}

inline Vector3 SafeNormalize(const Vector3& value, const Vector3& fallback = Vector3(0.0f), float epsilon = 0.000001f)
{
	float lengthSquared = glm::length2(value);
	if (!std::isfinite(lengthSquared) || lengthSquared <= epsilon)
		return fallback;
	return value * glm::inversesqrt(lengthSquared);
}

inline Vector2 SafeNormalize(const Vector2& value, const Vector2& fallback = Vector2(0.0f), float epsilon = 0.000001f)
{
	float lengthSquared = glm::length2(value);
	if (!std::isfinite(lengthSquared) || lengthSquared <= epsilon)
		return fallback;
	return value * glm::inversesqrt(lengthSquared);
}



#pragma region Debug OpenGl Call
#ifdef ENABLE_ASSERTS
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
#else
#define GLCall(x)	\
	x
#endif
#pragma endregion
