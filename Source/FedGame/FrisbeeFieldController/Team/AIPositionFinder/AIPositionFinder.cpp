#include <FedPCH.h>
#include <Resources/ResourceManager.h>
#include <Transform/Transform.h>
#include <Game/GameManager.h>
#include "../Team.h"
#include <FrisbeeFieldController/FrisbeeFieldController.h>
#include "AIPositionFinder.h"
namespace Fed {
	AIPositionFinder::AIPositionFinder(const Team* team) : m_Team(team), m_BestPosition(nullptr), m_UpdateTimer(0.0f)
	{
		// Fill in Positions
		int zSign = (team->GetColor() == TeamColor::Blue ? 1 : -1); // Get Opposite Side of Goal

		// Field is 40 x 60 in dimensions
		// Will only consider 30 x 25, in spacing of 5. 6 by 5 positions
		float x1 = -15.0f;
		float z1 =   0.0f;
		float x2 =  15.0f;
		float z2 =  25.0f * zSign;
		float spacing = 5.0f;

		m_Positions.reserve((glm::abs(z2) - glm::abs(z1)) * (x2 - x1) / spacing);
		for (float z = z1; std::abs(z) <= std::abs(z2); z += spacing * zSign) 
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

	void AIPositionFinder::Update()
	{
		m_UpdateTimer -= Game.DeltaTime();
		if (m_UpdateTimer <= 0) {
			UpdateBestPosition();
			m_UpdateTimer = UPDATE_FREQUENCY;
		}
	}

	void AIPositionFinder::UpdateBestPosition()
	{
		float bestScore = -1;
		FieldPosition* bestPos = nullptr;
		Vector3 fedoraPos = m_Team->GetFieldController()->GetFedoraPosition();
		const Team* enemyTeam = m_Team->GetFieldController()->GetEnemyTeam(m_Team->GetColor());
		float fedoraRange = m_Team->GetFieldController()->GetFedoraRange();

		for (FieldPosition& pos : m_Positions) {
			// Reward spots far from fedora
			float dist = glm::length(pos.Position - fedoraPos);
			float distScore = dist * 0.03f;

			// Reward spots within range of goal
			float distGoal = m_Team->GetFieldController()->GetCourt()->GetDistSqrToGoal(pos.Position, enemyTeam);
			float nearGoalScore = (distGoal < fedoraRange * fedoraRange ? 1.0f : 0.0f);

			// TODO: Reward spots that will not be intercepted from

			pos.Score = 1 + distScore + nearGoalScore;

			// Finding Best Position
			if (pos.Score > bestScore) {
				bestScore = pos.Score;
				bestPos = &pos;
			}
		}
		m_BestPosition = bestPos;
	}

}