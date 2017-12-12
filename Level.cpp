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

	mCurrentState = running;
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
		//used for testing purposes
		/*if (InputManager::Instance()->KeyPressed(SDL_SCANCODE_X)) {

			mPlayer->WasHit();
			mSideBar->SetLives(mPlayer->Lives());

			mPlayerHit = true;
			mPlayerRespawnTimer = 0.0f;
			mPlayer->Active(false);
		}*/
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

//current stat
Level::LEVEL_STATES Level::State() {

	return mCurrentState;
}

//update
void Level::Update() {

	if (!mStageStarted) {

		HandleStartLabels();
	}
	else {

		HandleCollisions();

		if (mPlayerHit) {

			HandlePlayerDeath();
		}
		else {
			//used for testing purposes
			/*if (InputManager::Instance()->KeyPressed(SDL_SCANCODE_N)) {

				mCurrentState = finished;
			}*/
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

		if (mPlayerHit) {

			if (mPlayerRespawnTimer >= mPlayerRespawnLabelOnScreen)
				mReadyLabel->Render();

			if (mGameOverTimer >= mGameOverLabelOnScreen) {

				mGameOverLabel->Render();
			}
		}
	}
}