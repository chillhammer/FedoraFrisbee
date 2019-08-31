#include <FedPCH.h>
#include <Resources/ResourceManager.h>
#include <Transform/Transform.h>
#include "../Team.h"
#include "AIPositionFinder.h"
namespace Fed {
	AIPositionFinder::AIPositionFinder(const Team* team) : m_Team(team), m_BestPosition(nullptr)
	{
		// Fill in Positions
		int zSign = (team->GetColor() == TeamColor::Blue ? -1 : 1); // Get Opposite Side of Goal

		// Field is 40 x 60 in dimensions
		// Will only consider 30 x 25, in spacing of 5. 6 by 5 positions
		float x1 = -15.0f;
		float z1 =   0.0f;
		float x2 =  15.0f;
		float z2 =  25.0f * zSign;
		float spacing = 5.0f;

		m_Positions.reserve((z2 - z1) * (x2 - x1) / spacing);

		for (float z = z1; glm::abs(z) <= glm::abs(z2); z += spacing * zSign) 
		{
			for (float x = x1; x <= x2; x += spacing) 
			{
				float score = 1.0f;
				m_Positions.emplace_back(Vector3(x, 0, z), score);
			}
		}
	}

	// Renders each spot
	void AIPositionFinder::DebugRender() const
	{
		ModelPtr box = Resources.GetModel("WoodenBox");
		ShaderPtr debug = Resources.GetShader("Debug");
		Transform trans;
		for (FieldPosition pos : m_Positions) {
			trans.Position = pos.Position;
			trans.Scale = Vector3(pos.Score, pos.Score, pos.Score);
			box->Draw(debug, trans.GetMatrix());
		}
	}

}