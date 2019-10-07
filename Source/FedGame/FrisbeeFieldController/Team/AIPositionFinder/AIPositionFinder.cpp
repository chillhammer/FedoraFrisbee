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
		int zSign = (team->GetColor() == TeamColor::Blue ? -1 : 1); // Get Opposite Side of Goal

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
		ShaderPtr bestShader = Resources.GetShader("Model");
		Transform trans;

		// Normalize scale only for debug rendering purposes
		float highScore = 0.0f;
		for (const FieldPosition& pos : m_Positions) {
			highScore = glm::max(pos.Score, highScore);
		}

		for (const FieldPosition& pos : m_Positions) {
			ShaderPtr shader = (&pos == m_BestPosition ? bestShader : debug);
			float scale = pos.Score / (highScore * 0.5f);
			trans.Position = pos.Position;
			trans.Scale = Vector3(scale, scale, scale);
			box->Draw(shader, trans.GetMatrix());
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

	Vector3 AIPositionFinder::GetBestPosition() const
	{
		return m_BestPosition->Position;
	}

	void AIPositionFinder::UpdateBestPosition()
	{
		float bestScore = -1;
		FrisbeeFieldController* controller = m_Team->GetFieldController();
		FieldPosition* bestPos = nullptr;
		Vector3 fedoraPos = controller->GetFedoraPosition();
		const Team* enemyTeam = controller->GetEnemyTeam(m_Team->GetColor());
		float fedoraRange = controller->GetFedoraRange();

		for (FieldPosition& pos : m_Positions) {
			// Reward spots far from fedora
			float dist = glm::length(pos.Position - fedoraPos);
			float distScore = dist * 0.03f;

			// Reward spots within range of goal
			Vector3 goalPos = controller->GetCourt()->GetGoalPosition(enemyTeam);
			float goalDistSqr = glm::length2(goalPos - pos.Position);
			float nearGoalScore = 0.0f; 
			if (goalDistSqr < fedoraRange * fedoraRange) {
				// Only calculate expensive intercept check if within goal range
				bool canScoreGoal = !controller->CanEnemyInterceptFedoraThrow(m_Team, pos.Position, goalPos);
				if (canScoreGoal)
					nearGoalScore = 0.5f;
			}

			// Reward spots that will not be intercepted from
			bool canPass = !controller->CanEnemyInterceptFedoraThrow(m_Team, fedoraPos, pos.Position);
			float passScore = (canPass ? 2.0f : 0.0f);

			//distScore = 0.0f;
			// pass score currently always returning true
			//passScore = 0.0f;
			// nearGoalScore = 0.0f;

			pos.Score = 1 + distScore + nearGoalScore + passScore;

			// Finding Best Position
			if (pos.Score > bestScore) {
				bestScore = pos.Score;
				bestPos = &pos;
			}
		}
		m_BestPosition = bestPos;
	}

}