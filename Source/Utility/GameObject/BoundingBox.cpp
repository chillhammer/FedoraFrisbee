#include <FedPCH.h>
#include <Resources/ResourceManager.h>
#include <Transform/Transform.h>
#include "BoundingBox.h"

namespace Fed
{
	BoundingBox::BoundingBox() 
		:	m_Center(0, 0, 0), m_HalfExtents(0, 0, 0),
			m_DebugBox(Resources.GetModel("WoodenBox"))
	{

	}
	BoundingBox::BoundingBox(const BoundingBox & other)
	{
		m_Center = other.m_Center;
		m_HalfExtents = other.m_HalfExtents;
		//m_DebugBox = other.m_DebugBox;	Can't Draw Clones
	}
	// Returns if bounding box is non-existent
	bool BoundingBox::IsEmpty() const
	{
		return m_HalfExtents.x == 0.f || m_HalfExtents.y == 0.f || m_HalfExtents.z == 0.f;
	}
	// Returns if two bounding boxes intersect via AABB
	bool BoundingBox::IsIntersecting(const Transform& myTrans, const Transform& otherTrans, const BoundingBox & other) const
	{
		if (IsEmpty() || other.IsEmpty())
			return false;
		Vector3 mins1 = myTrans.GetGlobalPosition() + m_Center - m_HalfExtents * myTrans.Scale;
		Vector3 maxs1 = myTrans.GetGlobalPosition() + m_Center + m_HalfExtents * myTrans.Scale;
		Vector3 mins2 = otherTrans.GetGlobalPosition() + other.m_Center - other.m_HalfExtents * otherTrans.Scale;
		Vector3 maxs2 = otherTrans.GetGlobalPosition() + other.m_Center + other.m_HalfExtents * otherTrans.Scale;
		if (mins1.x < maxs2.x && mins1.y < maxs2.y && mins1.z < maxs2.z)
		{
			if (maxs1.x > mins2.x && maxs1.y > mins2.y && maxs1.z > mins2.z)
			{
				return true;
			}
		}
		return false;
	}
	// Get direction to other bounding box, north, west, south, east. Must not be intersecting
	// Made for collision sliding
	Vector3 BoundingBox::GetSlidingDirection(const Transform & myTrans, const Transform & otherTrans, 
		const BoundingBox & other, const Vector3& dir) const
	{
		if (IsIntersecting(myTrans, otherTrans, other))
		{
			return Vector3(0, 0, 0);
		}
		//ASSERT(!IsIntersecting(myTrans, otherTrans, other), "Sliding must not be intersecting");
		Vector3 mins1 = myTrans.GetGlobalPosition() + m_Center - m_HalfExtents * myTrans.Scale;
		Vector3 maxs1 = myTrans.GetGlobalPosition() + m_Center + m_HalfExtents * myTrans.Scale;
		Vector3 mins2 = otherTrans.GetGlobalPosition() + other.m_Center - other.m_HalfExtents * otherTrans.Scale;
		Vector3 maxs2 = otherTrans.GetGlobalPosition() + other.m_Center + other.m_HalfExtents * otherTrans.Scale;
		Vector3 newDir = dir;
		if (mins1.x > maxs2.x || mins2.x > maxs1.x)
		{
			newDir.x = 0;
		}
		else if (mins1.z > maxs2.z || mins2.z > maxs1.z)
		{
			newDir.z = 0;
		}
		return newDir;
	}
	// Can change size and position to customize bounding boxes
	void BoundingBox::SetParameters(Vector3 center, Vector3 halfExtents)
	{
		m_Center = center;
		m_HalfExtents = halfExtents;
	}
	// Draws box covering space for debug pursposes
	void BoundingBox::DebugDraw(const Transform & transform) const
	{
		Vector3 center = transform.GetGlobalPosition() + m_Center;
		Vector3 scale = transform.Scale * (2.f * m_HalfExtents);
		Transform debugTransform;
		debugTransform.Position = center;
		debugTransform.Scale = scale;
		m_DebugBox->Draw(Resources.GetShader("Debug"), debugTransform.GetMatrix());
	}
}