#ifndef _DRONE_H
#define _DRONE_H
#include "Enemy.h"

class Drone : public Enemy {

public:

	AnimatedTexture* mDeathAnimation;

private:

	AudioManager* mAudio;

	static std::vector<std::vector<Vector2>> sDivePaths;

	bool mEscort;

private:

	Vector2 LocalFormationPosition();

	void HandleDiveState();
	void HandleDeadState();

	void RenderDiveState();
	void RenderDeadState();

public:

	static void CreateDivePaths();

	void Hit(PhysEntity* other) override;

	Drone(int index, int path, bool challengeStage);
	~Drone();

	void Dive(int type = 0);

};

#endif // !_DRONE_H