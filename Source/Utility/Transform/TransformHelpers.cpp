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
}