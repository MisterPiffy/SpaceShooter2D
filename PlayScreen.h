#ifndef _PLAYSCREEN_H
#define _PLAYSCREEN_H
#include "GameEntity.h"
#include "Timer.h"
#include "Level.h"

class PlayScreen : public GameEntity {

private:

	Timer* mTimer;
	InputManager* mInput;
	AudioManager* mAudio;

	BackgroundStars* mStars;

	PlaySideBar* mSideBar;

	float mLevelStartTimer;
	float mLevelStartDelay;
	float mStartDelay;
	float mStartTimer;

	Texture* mStartLabel;

	bool mGameStarted;

	Level* mLevel;
	bool mLevelStarted;
	int mCurrentStage;

	Player* mPlayer;

private:

	void StartNextLevel();

public:

	PlayScreen();
	~PlayScreen();

	void StartNewGame();

	bool GameOver();

	void Update();

	void Render();
};

#endif // !_PLAYSCREEN_H