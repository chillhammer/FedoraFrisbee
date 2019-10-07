#pragma once
#include <vector>

namespace Fed
{
	// Used by AI agents to find the best position to move towards to. Currently used for those without fedora to recieve a pass
	// Currently hardcoded to only find positions within Court
	class AIPositionFinder
	{
	public:
		AIPositionFinder(const class Team* team);
		void DebugRender() const;
		void Update();
		Vector3 GetBestPosition() const;
	private:
		AIPositionFinder() {};
		struct FieldPosition {
			Vector3 Position;
			float	Score;
			FieldPosition() : Position(0, 0, 0), Score(0) {}
			FieldPosition(Vector3 pos, float score) : Position(pos), Score(score) {}
		};
		void UpdateBestPosition();

		const Team* m_Team;
		const FieldPosition* m_BestPosition;
		std::vector<FieldPosition> m_Positions;
		float m_UpdateTimer;
		const float UPDATE_FREQUENCY = 0.25f;
	};
}