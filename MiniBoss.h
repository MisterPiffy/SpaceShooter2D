#ifndef _MINIBOSS_H
#define _MINIBOSS_H
#include "Enemy.h"
#include "CaptureBeam.h"

class MiniBoss : public Enemy {

public:

	AnimatedTexture* mDeathAnimation;

private:

	AudioManager* mAudio;

	static std::vector<std::vector<Vector2>> sDivePaths;

	bool mCaptureDive;

	int mCurrentPath;

	bool mCapturing;

	CaptureBeam* mCaptureBeam;

private:

	Vector2 LocalFormationPosition();

	void HandleCaptureBeam();

	void HandleDiveState();
	void HandleDeadState();

	void RenderDiveState();
	void RenderDeadState();

public:

	static void CreateDivePaths();

	void Hit(PhysEntity* other) override;

	MiniBoss(int index, int path, bool challengeStage);
	~MiniBoss();

	void Dive(int type = 0);
};

#endif // !_MINIBOSS_H