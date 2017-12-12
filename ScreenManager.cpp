#include "ScreenManager.h"

ScreenManager* ScreenManager::sInstance = NULL;

ScreenManager* ScreenManager::Instance() {

	if (sInstance == NULL) {
		sInstance = new ScreenManager();
	}

	return sInstance;
}

void ScreenManager::Release() {

	delete sInstance;
	sInstance = NULL;
}

//constructor
ScreenManager::ScreenManager() {

	mInput = InputManager::Instance();

	mStartScreen = new StartScreen();

	mPlayScreen = new PlayScreen();

	mStars = BackgroundStars::Instance();
	mStars->Scroll(false);

	mCurrentScreen = start;
}

//destructor
ScreenManager::~ScreenManager() {

	mInput = NULL;

	delete mStartScreen;
	mStartScreen = NULL;

	delete mPlayScreen;
	mPlayScreen = NULL;

	BackgroundStars::Release();
	mStars = NULL;
}

//update
void ScreenManager::Update() {

	mStars->Update();
	switch (mCurrentScreen) {

	case start:

		mStartScreen->Update();
		if (mInput->KeyPressed(SDL_SCANCODE_RETURN) && mStartScreen->Selected() == 0) {
			mStars->Scroll(true);
			mCurrentScreen = play;
			mPlayScreen->StartNewGame();
		}
		break;

	case play:

		mPlayScreen->Update();
		if (mPlayScreen->GameOver()) {

			mCurrentScreen = start;
			mStars->Scroll(false);
		}
		break;
	}
}

//render
void ScreenManager::Render() {

	mStars->Render();
	switch (mCurrentScreen) {

	case start:

		mStartScreen->Render();
		break;

	case play:

		mPlayScreen->Render();
		break;
	}
}