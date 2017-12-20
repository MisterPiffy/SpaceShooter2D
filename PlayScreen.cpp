#include "PlayScreen.h"

//constructor
PlayScreen::PlayScreen() {

	mTimer = Timer::Instance();
	mInput = InputManager::Instance();
	mStars = BackgroundStars::Instance();
	mAudio = AudioManager::Instance();
	mStars->Scroll(true);

	mSideBar = new PlaySideBar();
	mSideBar->Parent(this);
	mSideBar->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.87f, Graphics::Instance()->SCREEN_HEIGHT * 0.05f));

	mStartLabel = new Texture("START", "fonts/cour.ttf", 64, { 150, 0, 0 });
	mStartLabel->Parent(this);
	mStartLabel->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.4f, Graphics::Instance()->SCREEN_HEIGHT * 0.5f));

	mLevel = NULL;
	mLevelStartDelay = 1.0f;
	mLevelStarted = false;
	mStartDelay = 3.0f;

	mPlayer = NULL;

	Enemy::CreatePaths();
	Bruiser::CreateDivePaths();
	Drone::CreateDivePaths();
	MiniBoss::CreateDivePaths();
}

//destructor
PlayScreen::~PlayScreen() {

	mTimer = NULL;
	mInput = NULL;

	mStars = NULL;
	mAudio = NULL;

	delete mSideBar;
	mSideBar = NULL;

	delete mStartLabel;
	mStartLabel = NULL;

	delete mLevel;
	mLevel = NULL;

	delete mPlayer;
	mPlayer = NULL;
}

//start next level
void PlayScreen::StartNextLevel() {

	mCurrentStage++;
	mLevelStartTimer = 0.0f;
	mLevelStarted = true;
	
	delete mLevel;
	mLevel = new Level(mCurrentStage, mSideBar, mPlayer);
}

//set up new game
void PlayScreen::StartNewGame() {

	//set up player
	delete mPlayer;
	mPlayer = new Player();
	mPlayer->Parent(this);
	mPlayer->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.4f, Graphics::Instance()->SCREEN_HEIGHT * 0.8f));
	mPlayer->Active(false);

	mSideBar->SetHighScore(30000);
	mSideBar->SetLives(mPlayer->Lives());
	mSideBar->SetPlayerScore(mPlayer->Score());
	mSideBar->SetLevel(0);

	mGameStarted = false;
	mLevelStarted = false;
	mLevelStartTimer = 0.0f;
	mCurrentStage = 0;
	mAudio->PlayMusic("audio/spaceChase.wav");
}

//game over take back to main menu
bool PlayScreen::GameOver() {

	if (!mLevelStarted)
		return false;

	return (mLevel->State() == Level::gameover);
}

//update
void PlayScreen::Update() {

	if (mGameStarted) {

		if (!mLevelStarted) {

			mLevelStartTimer += mTimer->DeltaTime();
			if (mLevelStartTimer >= mLevelStartDelay) {
				StartNextLevel();
			}
		}

	}
	else {

		if (!mGameStarted) {
			mStartTimer += mTimer->DeltaTime();
			if (mStartTimer >= mStartDelay) {
				mGameStarted = true;
			}
		}
	}

	if (mGameStarted) {

		if (mCurrentStage > 0) {

			mSideBar->Update();
		}
		if (mLevelStarted) {

			mLevel->Update();

			if (mLevel->State() == Level::finished) {

				mLevelStarted = false;
			}
		}

		mPlayer->Update();
	}
}

//render
void PlayScreen::Render() {

	if (!mGameStarted) {

		mStartLabel->Render();
	}

	if (mGameStarted) {

		if (mLevelStarted) {
			mLevel->Render();
		}

		mPlayer->Render();
	}

	mSideBar->Render();
}
