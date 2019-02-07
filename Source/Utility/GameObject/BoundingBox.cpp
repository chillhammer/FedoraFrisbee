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
	// Returns if bounding box is non-existent
	bool BoundingBox::IsEmpty() const
	{
		return m_HalfExtents.x == 0.f || m_HalfExtents.y == 0.f || m_HalfExtents.z == 0.f;
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
		Vector3 center = transform.Position + m_Center;
		Vector3 scale = transform.Scale * (2.f * m_HalfExtents);
		Transform debugTransform;
		debugTransform.Position = center;
		debugTransform.Scale = scale;
		m_DebugBox->Draw(Resources.GetShader("Model"), debugTransform.GetMatrix());
	}
}