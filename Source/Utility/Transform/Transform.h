#pragma once
#include <FedPCH.h>
#include <gtx/quaternion.hpp>

/**
	Data related to change runtime rendering/physics.
	Instancable objects must have a Transform.
**/
namespace Fed
{
	Vector3 GetEulerAngles(const Quaternion& quat);
	Quaternion ConvertEulerToQuaternion(const Vector3& eulerAngles);

	struct Transform
	{
		Vector3 Position;
		Quaternion Rotation;
		Vector3 Scale;

		Matrix4x4 GetMatrix() const
		{
			Matrix4x4 posMat(1.f);
			glm::translate(posMat, Position);

			Matrix4x4 rotMat(1.f);
			glm::toMat4(Rotation);

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
			rotMat = glm::toMat4(Rotation);
			//rotMat = glm::rotate(GetPitch(), front, );
			Vector4 heading = glm::rotate(Rotation, front);
			//LOG("Heading: {0}, {1}, {2}, {3}", heading.x, heading.y, heading.z, heading.w);
			//LOG("Rotation: {0}, {1}, {2}, {3}", Rotation.x, Rotation.y, Rotation.z, Rotation.w);
			return rotMat *  front;
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
			return GetEulerAngles(Rotation).x;
		}
		float GetYaw() const
		{
			return GetEulerAngles(Rotation).y;
		}
		float GetRoll() const
		{
			return GetEulerAngles(Rotation).z;
		}
		void SetPitch(float angle)
		{
			//LOG("Pitch being set to: {0}", angle);
			Vector3 eulers = GetEulerAngles(Rotation);
			eulers.x = angle;
			Rotation = ConvertEulerToQuaternion(eulers);
		}
		void SetYaw(float angle)
		{
			Vector3 eulers = GetEulerAngles(Rotation);
			eulers.y = angle;
			Rotation = ConvertEulerToQuaternion(eulers);
		}
		void SetRoll(float angle)
		{
			Vector3 eulers = GetEulerAngles(Rotation);
			eulers.z = angle;
			Rotation = ConvertEulerToQuaternion(eulers);
		}
	};
}