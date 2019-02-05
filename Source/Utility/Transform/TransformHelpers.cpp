#include <FedPCH.h>
#include "Transform.h"

namespace Fed
{
	Vector3 GetEulerAngles(const Quaternion& quat)
	{
		return glm::eulerAngles(quat);
	}
	Quaternion ConvertEulerToQuaternion(const Vector3& eulerAngles)
	{
		Quaternion quat(eulerAngles);
		return quat;
	}
	float ProcessAngle(float angle)
	{
		angle = std::fmod(angle, 360);
		if (angle < 0) angle += 360; // Fmod doesn't account for negative numbers
		return angle;
	}
	Vector3 ProcessAngleVector(Vector3 angles)
	{
		angles.x = ProcessAngle(angles.x);
		angles.y = ProcessAngle(angles.y);
		angles.z = ProcessAngle(angles.z);
		return angles;
	}
	// Lerps angle in degrees
	float LerpAngle(float start, float end, float amount)
	{
		start = ProcessAngle(start);
		end = ProcessAngle(end);
		float shortest_angle = std::fmod( std::fmod(end - start, 360)  + 540, 360) - 180;
		return ProcessAngle(start + shortest_angle * amount);
	}
}