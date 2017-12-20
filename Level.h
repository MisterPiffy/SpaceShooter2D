#ifndef _LEVEL_H
#define _LEVEL_H
#include "PlaySideBar.h"
#include "BackgroundStars.h"
#include "Player.h"
#include "Drone.h"
#include "Bruiser.h"
#include "MiniBoss.h"
#include "tinyxml2.h"

using namespace tinyxml2;

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

	float mLevelTimer;
	float mNextLevelDelay;

	LEVEL_STATES mCurrentState;

	bool mChallengeStage;

	Formation* mFormation;

	static const int MAX_DRONES = 16;
	int mDroneCount;

	static const int MAX_BRUISER = 20;
	int mBruiserCount;

	static const int MAX_MINIBOSS = 4;
	int mMiniBossCount;

	Drone* mFormationDrone[MAX_DRONES];
	Bruiser* mFormationBruiser[MAX_BRUISER];
	MiniBoss* mFormationMiniBoss[MAX_MINIBOSS];

	std::vector<Enemy*> mEnemies;

	XMLDocument mSpawningPatterns;
	int mCurrentFlyinPriority;
	int mCurrentFlyinIndex;

	bool mSpawningFinished;
	float mSpawnDelay;
	float mSpawnTimer;

	Drone* mDivingDrone;
	bool mSkipFirstDrone;
	float mDroneDiveDelay;
	float mDroneDiveTimer;

	Bruiser* mDivingBruiser;
	Bruiser* mDivingBruiser2;
	float mBruiserDiveDelay;
	float mBruiserDiveTimer;

	MiniBoss* mDivingMiniBoss;
	bool mCaptureDive;
	bool mSkipFirstBoss;
	float mBossDiveDelay;
	float mBossDiveTimer;

private:

	void StartStage();
	void HandleStartLabels();
	void HandleCollisions();
	void HandlePlayerDeath();

	bool EnemyFlyingIn();
	void HandleEnemySpawning();
	void HandleEnemyFormation();
	void HandleEnemyDiving();

public:

	Level(int stage, PlaySideBar* sideBar, Player* player);
	~Level();

	LEVEL_STATES State();

	void Update();

	void Render();

};

#endif // !_LEVEL_H