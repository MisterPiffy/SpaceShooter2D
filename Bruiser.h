#ifndef BRUISER_H
#define BRUISER_H
#include "Enemy.h"

class Bruiser : public Enemy {

public:

	AnimatedTexture* mDeathAnimation;

private:

	AudioManager* mAudio;

	static std::vector<std::vector<Vector2>> sDivePaths;

	bool mDiver;

private:

	void PathComplete();
	void FlyInComplete();

	Vector2 LocalFormationPosition();

	void HandleDiveState();
	void HandleDeadState();

	void RenderDiveState();
	void RenderDeadState();

public:

	static void CreateDivePaths();

	void Hit(PhysEntity* other) override;

	Bruiser(int index, int path, bool challengeStage, bool diver);
	~Bruiser();
};

#endif // !BRUISER_H