#pragma once
#include <Graphics/Model/Model.h>

namespace Fed
{
	/**
		All GameObjects must have a bounding box and this allows for collision.
		Children will set parameters, if not, they cannot collide.
	**/
	class BoundingBox
	{
	public:
		BoundingBox();

		bool IsEmpty() const;
		bool IsIntersecting(const Transform& myTrans, const Transform& otherTrans, const BoundingBox& other) const;
		void SetParameters(Vector3 center, Vector3 halfExtents);
		void DebugDraw(const struct Transform&) const;
	private:
		Vector3 m_Center;
		Vector3 m_HalfExtents;
		ModelPtr m_DebugBox;
	};
}