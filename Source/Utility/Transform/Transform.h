#pragma once
#include <FedPCH.h>
#include <gtx/quaternion.hpp>
#include <gtx/euler_angles.hpp>

/**
	Data related to change runtime rendering/physics.
	Instancable objects must have a Transform.
**/
namespace Fed
{
	Vector3 GetEulerAngles(const Quaternion& quat);
	Quaternion ConvertEulerToQuaternion(const Vector3& eulerAngles);
	float ProcessAngle(float angle);

	struct Transform
	{
		Vector3 Position;
		Vector3 Rotation;
		Vector3 Scale;

		Matrix4x4 GetMatrix() const
		{
			Matrix4x4 posMat(1.f);
			glm::translate(posMat, Position);

			Matrix4x4 rotMat(1.f);
			rotMat = glm::eulerAngleXYZ(Rotation.x, Rotation.y, Rotation.z);

			Matrix4x4 scaleMat(1.f);
			glm::scale(scaleMat, Scale);
			return posMat * rotMat * scaleMat;
		}

		Vector3 GetHeading() const
		{
			Vector4 front(1.f, 0.f, 0.f, 0.f);
			Vector4 up(0.f, 1.f, 0.f, 0.f);
			Vector4 side(0.f, 0.f, 1.f, 0.f);
			Matrix4x4 rotMat(1.f);

			Vector3 heading;
			heading.x = cos(glm::radians(GetYaw())) * cos(glm::radians(GetPitch()));
			heading.y = sin(glm::radians(GetPitch()));
			heading.z = sin(glm::radians(GetYaw())) * cos(glm::radians(GetPitch()));
			heading = glm::normalize(heading);
			//LOG("Heading: {0}, {1}, {2}, {3}", heading.x, heading.y, heading.z, heading.w);
			//LOG("Rotation: {0}, {1}, {2}, {3}", Rotation.x, Rotation.y, Rotation.z, Rotation.w);
			return heading;
		}
		Vector3 GetSide() const
		{
			Vector3 up(0.f, 1.f, 0.f);
			return glm::normalize(glm::cross(GetHeading(), up));
		}
		Vector3 GetUp() const
		{
			return glm::normalize(glm::cross(GetSide(), GetHeading()));
		}

		float GetPitch() const
		{
			return Rotation.x;
		}
		float GetYaw() const
		{
			return Rotation.y;
		}
		float GetRoll() const
		{
			return Rotation.z;
		}
		void SetPitch(float angle)
		{
			angle = ProcessAngle(angle);
			Rotation.x = angle;
		}
		void SetYaw(float angle)
		{
			angle = ProcessAngle(angle);
			Rotation.y = angle;
		}
		void SetRoll(float angle)
		{
			angle = ProcessAngle(angle);
			Rotation.z = angle;
		}
	};
}