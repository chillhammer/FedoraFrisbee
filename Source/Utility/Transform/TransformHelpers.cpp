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
		//if (angle > 360) angle = 0;
		//if (angle < 0) angle = 360;
		return angle;
	}
}