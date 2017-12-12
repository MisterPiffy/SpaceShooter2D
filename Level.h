#ifndef _LEVEL_H
#define _LEVEL_H
#include "PlaySideBar.h"
#include "BackgroundStars.h"
#include "Player.h"

class Level : public GameEntity {

public:

	enum LEVEL_STATES{running, finished, gameover};

private:

	Timer* mTimer;
	PlaySideBar* mSideBar;
	BackgroundStars* mStars;

	int mStage;
	bool mStageStarted;

	float mLabeltimer;

	Texture* mStageLabel;
	Scoreboard* mStageNumber;
	float mStageLabelOnScreen;
	float mStageLabelOffScreen;

	Texture* mReadyLabel;
	float mReadyLabelOnScreen;
	float mReadyLabelOffScreen;

	Player* mPlayer;
	bool mPlayerHit;
	float mPlayerRespawnDelay;
	float mPlayerRespawnTimer;
	float mPlayerRespawnLabelOnScreen;

	Texture* mGameOverLabel;
	bool mGameOver;
	float mGameOverDelay;
	float mGameOverTimer;
	float mGameOverLabelOnScreen;

	LEVEL_STATES mCurrentState;

private:

	void StartStage();
	void HandleStartLabels();
	void HandleCollisions();
	void HandlePlayerDeath();

public:

	Level(int stage, PlaySideBar* sideBar, Player* player);
	~Level();

	LEVEL_STATES State();

	void Update();

	void Render();

};

#endif // !_LEVEL_H