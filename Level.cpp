#include "Level.h"

//constructor
Level::Level(int stage, PlaySideBar* sideBar, Player* player) {

	mTimer = Timer::Instance();
	mSideBar = sideBar;
	mSideBar->SetLevel(stage);
	mStars = BackgroundStars::Instance();

	mStage = stage;
	mStageStarted = false;

	mLabeltimer = 0.0f;

	mStageLabel = new Texture("STAGE", "fonts/cour.ttf", 32, { 75, 75, 200 });
	mStageLabel->Parent(this);
	mStageLabel->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.35f, Graphics::Instance()->SCREEN_HEIGHT * 0.5f));

	mStageNumber = new Scoreboard();
	mStageNumber->Score(mStage);
	mStageNumber->Parent(this);
	mStageNumber->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f, Graphics::Instance()->SCREEN_HEIGHT * 0.5f));

	mStageLabelOnScreen = 0.0f;
	mStageLabelOffScreen = 1.5f;

	mReadyLabel = new Texture("READY", "fonts/cour.ttf", 32, {200, 0, 0 });
	mReadyLabel->Parent(this);
	mReadyLabel->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.4f, Graphics::Instance()->SCREEN_HEIGHT * 0.5f));

	mReadyLabelOnScreen = mStageLabelOffScreen;
	mReadyLabelOffScreen = mReadyLabelOnScreen + 3.0f;

	mPlayer = player;
	mPlayerHit = false;
	mPlayerRespawnDelay = 3.0f;
	mPlayerRespawnTimer = 0.0f;
	mPlayerRespawnLabelOnScreen = 2.0f;

	mGameOverLabel = new Texture("GAME OVER", "fonts/cour.ttf", 32, {150, 0, 0});
	mGameOverLabel->Parent(this);
	mGameOverLabel->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.4f, Graphics::Instance()->SCREEN_HEIGHT * 0.5f));

	mGameOver = false;
	mGameOverDelay = 6.0f;
	mGameOverTimer = 0.0f;
	mGameOverLabelOnScreen = 1.0f;

	mLevelTimer = 0.0f;
	mNextLevelDelay = 15.0f;

	mCurrentState = running;

	mDroneCount = 0;
	mBruiserCount = 0;
	mMiniBossCount = 0;

	std::string fullPath = SDL_GetBasePath();
	fullPath.append("Data/Level1.xml");
	mSpawningPatterns.LoadFile(fullPath.c_str());

	mChallengeStage = mSpawningPatterns.FirstChildElement("Level")->FirstChildElement()->BoolAttribute("value");

	if (!mChallengeStage) {

		mFormation = new Formation();
		mFormation->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.4f, 150.0f));
		Enemy::SetFormation(mFormation);

		for (int i = 0; i < MAX_DRONES; i++) {
			mFormationDrone[i] = NULL;
		}
		for (int i = 0; i < MAX_BRUISER; i++) {
			mFormationBruiser[i] = NULL;
		}
		for (int i = 0; i < MAX_MINIBOSS; i++) {
			mFormationMiniBoss[i] = NULL;
		}
	}

	mCurrentFlyinPriority = 0;
	mCurrentFlyinIndex = 0;
	mSpawningFinished = false;
	mSpawnDelay = 0.2f;
	mSpawnTimer = 0.0f;

	mDivingDrone = NULL;
	mSkipFirstDrone = false;
	mDroneDiveDelay = 1.0f;
	mDroneDiveTimer = 0.0f;

	mDivingBruiser = NULL;
	mDivingBruiser2 = NULL;
	mBruiserDiveDelay = 3.0f;
	mBruiserDiveTimer = 0.0f;

	mDivingMiniBoss = NULL;
	mCaptureDive = true;
	mSkipFirstBoss = true;
	mBossDiveDelay = 5.0f;
	mBossDiveTimer = 0.0f;
}

//destructor
Level::~Level() {

	mTimer = NULL;
	mSideBar = NULL;
	mStars = NULL;

	delete mStageLabel;
	mStageLabel = NULL;
	delete mStageNumber;
	mStageNumber = NULL;

	delete mReadyLabel;
	mReadyLabel = NULL;

	mPlayer = NULL;

	delete mGameOverLabel;
	mGameOverLabel = NULL;

	if (!mChallengeStage) {

		delete mFormation;
		mFormation = NULL;

		for (int i = 0; i < MAX_DRONES; i++) {
			delete mFormationDrone[i];
			mFormationDrone[i] = NULL;
		}
		for (int i = 0; i < MAX_BRUISER; i++) {
			delete mFormationBruiser[i];
			mFormationBruiser[i] = NULL;
		}
		for (int i = 0; i < MAX_MINIBOSS; i++) {
			delete mFormationMiniBoss[i];
			mFormationMiniBoss[i] = NULL;
		}
	}

	for (int i = 0; i < mEnemies.size(); i++) {

		delete mEnemies[i];
		mEnemies[i] = NULL;
	}
}

void Level::StartStage() {

	mStageStarted = true;
}

void Level::HandleStartLabels() {

	mLabeltimer += mTimer->DeltaTime();
	if (mLabeltimer >= mStageLabelOffScreen) {

		if (mStage > 1) {
			StartStage();
		}
		else {

			if (mLabeltimer >= mReadyLabelOffScreen) {
				StartStage();
				mPlayer->Active(true);
				mPlayer->Visible(true);
			}
		}
	}
}

//collisions
void Level::HandleCollisions() {

	if (!mPlayerHit) {

		if (mPlayer->WasHit()) {

			mSideBar->SetLives(mPlayer->Lives());
			mPlayerHit = true;
			mPlayerRespawnTimer = 0.0f;
			mPlayer->Active(false);
		}
	}
}

bool Level::EnemyFlyingIn() {

	for (int i = 0; i < MAX_DRONES; i++) {

		if (mFormationDrone[i] != NULL && mFormationDrone[i]->CurrentState() == Enemy::flyin) {
			return true;
		}
	}
	for (int i = 0; i < MAX_BRUISER; i++) {

		if (mFormationBruiser[i] != NULL && mFormationBruiser[i]->CurrentState() == Enemy::flyin) {
			return true;
		}
	}
	for (int i = 0; i < MAX_MINIBOSS; i++) {

		if (mFormationMiniBoss[i] != NULL && mFormationMiniBoss[i]->CurrentState() == Enemy::flyin) {
			return true;
		}
	}

	return false;
}

void Level::HandleEnemySpawning() {

	mSpawnTimer += mTimer->DeltaTime();
	if (mSpawnTimer >= mSpawnDelay) {

		XMLElement* element = mSpawningPatterns.FirstChildElement("Level")->FirstChild()->NextSiblingElement();
		bool spawned = false;
		bool priorityFound = false;

		while (element != NULL) {

			int priority = element->IntAttribute("priority");
			int path = element->IntAttribute("path");
			XMLElement* child = element->FirstChildElement();
			
			if (mCurrentFlyinPriority == priority) {

				priorityFound = true;
				for (int i = 0; i < mCurrentFlyinIndex && child != NULL; i++) {

					child = child->NextSiblingElement();
				}

				//spawn enemies
				if (child != NULL) {

					std::string type = child->Attribute("type");
					int index = child->IntAttribute("index");

					if (type.compare("Drone") == 0) {

						if (!mChallengeStage) {

							mFormationDrone[index] = new Drone(index, path, false);
							mDroneCount++;
						}
						else {
							mEnemies.push_back(new Drone(index, path, false));
						}
					}

					else if (type.compare("Bruiser") == 0) {

						if (!mChallengeStage) {

							mFormationBruiser[index] = new Bruiser(index, path, false, false);
							mBruiserCount++;
						}
						else {
							mEnemies.push_back(new Bruiser(index, path, false, false));
						}
					}
					else if (type.compare("MiniBoss") == 0) {

						if (!mChallengeStage) {

							mFormationMiniBoss[index] = new MiniBoss(index, path, false);
							mMiniBossCount++;
						}
						else {
							mEnemies.push_back(new MiniBoss(index, path, false));
						}
					}

					spawned = true;
				}
			}

			element = element->NextSiblingElement();
		}

		if (!priorityFound) {

			mSpawningFinished = true;
		}
		else {

			if (!spawned) {
				
				if (!EnemyFlyingIn()) {

					mCurrentFlyinPriority++;
					mCurrentFlyinIndex = 0;
				}
			}
			else {

				mCurrentFlyinIndex++;
			}
		}

		mSpawnTimer = 0.0f;
	}
}

void Level::HandleEnemyFormation() {

	mFormation->Update();

	for (int i = 0; i < MAX_DRONES; i++) {

		if (mFormationDrone[i] != NULL) {
			mFormationDrone[i]->Update();
		}
	}

	for (int i = 0; i < MAX_BRUISER; i++) {

		if (mFormationBruiser[i] != NULL) {
			mFormationBruiser[i]->Update();
		}
	}

	for (int i = 0; i < MAX_MINIBOSS; i++) {

		if (mFormationMiniBoss[i] != NULL) {
			mFormationMiniBoss[i]->Update();
		}
	}

	if (!mFormation->Locked()) {

		if (mDroneCount == MAX_DRONES && mBruiserCount == MAX_BRUISER && mMiniBossCount == MAX_MINIBOSS) {

			if (!EnemyFlyingIn()) {

				mFormation->Lock();
			}
		}
	}
	else {
		HandleEnemyDiving();
	}
}

//diver
void Level::HandleEnemyDiving() {

	if (mDivingDrone == NULL) {

		mDroneDiveTimer += mTimer->DeltaTime();

		if (mDroneDiveTimer >= mDroneDiveDelay) {

			bool skipped = false;
			for (int i = MAX_DRONES - 1; i >= 0; i--) {

				if (mFormationDrone[i]->CurrentState() == Enemy::formation) {

					if (!mSkipFirstDrone || (mSkipFirstDrone && skipped)) {

						mDivingDrone = mFormationDrone[i];
						mDivingDrone->Dive();
						mSkipFirstDrone = !mSkipFirstDrone;
						break;
					}

					skipped = true;
				}
			}

			mDroneDiveTimer = 0.0f;
		}
	}
	else {

		if (mDivingDrone->CurrentState() != Enemy::dive) {

			mDivingDrone = NULL;
		}
	}

	mBruiserDiveTimer += mTimer->DeltaTime();

	if (mBruiserDiveTimer >= mBruiserDiveDelay) {

		for (int i = MAX_BRUISER - 1; i >= 0; i--) {

			if (mFormationBruiser[i]->CurrentState() == Enemy::formation) {

				if (mDivingBruiser == NULL) {

					mDivingBruiser = mFormationBruiser[i];
					mDivingBruiser->Dive();
				}
				else if (mDivingBruiser2 == NULL) {

					mDivingBruiser2 = mFormationBruiser[i];
					mDivingBruiser2->Dive();
				}
				break;
			}
		}

		mBruiserDiveTimer = 0.0f;
	}

	if (mDivingBruiser != NULL && mDivingBruiser->CurrentState() != Enemy::dive) {
		mDivingBruiser = NULL;
	}
	if (mDivingBruiser2 != NULL && mDivingBruiser2->CurrentState() != Enemy::dive) {
		mDivingBruiser2 = NULL;
	}

	if (mDivingMiniBoss == NULL) {

		mBossDiveTimer += mTimer->DeltaTime();

		if (mBossDiveTimer >= mBossDiveDelay) {

			bool skipped = false;
			for (int i = MAX_MINIBOSS - 1; i >= 0; i--) {

				if (mFormationMiniBoss[i]->CurrentState() == Enemy::formation) {

					if (!mSkipFirstBoss || (mSkipFirstBoss && skipped)) {

						mDivingMiniBoss = mFormationMiniBoss[i];
						if (mCaptureDive) {

							mDivingMiniBoss->Dive(1);
						}
						else {

							mDivingMiniBoss->Dive();
							int index = mDivingMiniBoss->Index();
							int firstescortindex = (index % 2 == 0) ? (index * 2) : (index * 2 - 1);
							int secondescorindex = firstescortindex + 4;

							if (mFormationDrone[firstescortindex]->CurrentState() == Enemy::formation) {
								mFormationDrone[firstescortindex]->Dive(1);
							}
							if (mFormationDrone[secondescorindex]->CurrentState() == Enemy::formation) {
								mFormationDrone[secondescorindex]->Dive(1);
							}
						}
						mSkipFirstBoss = !mSkipFirstBoss;
						mCaptureDive = !mCaptureDive;
						break;
					}

					skipped = true;
				}
			}

			mBossDiveTimer = 0.0f;
		}
	}
	else {

		if (mDivingMiniBoss->CurrentState() != Enemy::dive) {

			mDivingMiniBoss = NULL;
		}
	}
}

//death
void Level::HandlePlayerDeath() {

	if (!mPlayer->IsAnimating()) {

		if (mPlayer->Lives() > 0) {

			if (mPlayerRespawnTimer == 0.0f)
				mPlayer->Visible(false);

			mPlayerRespawnTimer += mTimer->DeltaTime();
			if (mPlayerRespawnTimer >= mPlayerRespawnDelay) {

				mPlayer->Active(true);
				mPlayer->Visible(true);
				mPlayerHit = false;
			}
		}
		else {

			if (mGameOverTimer == 0.0f)
				mPlayer->Visible(false);

			mGameOverTimer += mTimer->DeltaTime();
			if (mGameOverTimer >= mGameOverDelay) {

				mCurrentState = gameover;
			}
		}
	}
}

//current state
Level::LEVEL_STATES Level::State() {

	return mCurrentState;
}

//update
void Level::Update() {

	mLevelTimer += mTimer->DeltaTime();

	if (!mStageStarted) {

		HandleStartLabels();
	}
	else {

		if (!mSpawningFinished) {
			HandleEnemySpawning();
		}
		if (!mChallengeStage) {
			HandleEnemyFormation();
		}

		for (int i = 0; i < mEnemies.size(); i++) {

			mEnemies[i]->Update();
		}

		HandleCollisions();

		if (mPlayerHit) {

			HandlePlayerDeath();
		}
		else {

			if (InputManager::Instance()->KeyDown(SDL_SCANCODE_W)) {
				mCurrentState = finished;
			}
		}
	}
}

//render
void Level::Render() {

	if (!mStageStarted) {

		if (mLabeltimer > mStageLabelOnScreen && mLabeltimer < mStageLabelOffScreen) {

			mStageLabel->Render();
			mStageNumber->Render();
		}
		else if (mLabeltimer > mReadyLabelOnScreen && mLabeltimer < mReadyLabelOffScreen) {

			mReadyLabel->Render();
		}
	}
	else {

		if (!mChallengeStage) {

			for (int i = 0; i < MAX_DRONES; i++) {

				if (mFormationDrone[i] != NULL) {
					mFormationDrone[i]->Render();
				}
			}

			for (int i = 0; i < MAX_BRUISER; i++) {

				if (mFormationBruiser[i] != NULL) {
					mFormationBruiser[i]->Render();
				}
			}

			for (int i = 0; i < MAX_MINIBOSS; i++) {

				if (mFormationMiniBoss[i] != NULL) {
					mFormationMiniBoss[i]->Render();
				}
			}
		}

		for (int i = 0; i < mEnemies.size(); i++) {

			mEnemies[i]->Render();
		}

		if (mPlayerHit) {

			if (mPlayerRespawnTimer >= mPlayerRespawnLabelOnScreen)
				mReadyLabel->Render();

			if (mGameOverTimer >= mGameOverLabelOnScreen) {

				mGameOverLabel->Render();
			}
		}
	}
}