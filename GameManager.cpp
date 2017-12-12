#include "GameManager.h"
#include <time.h>

GameManager* GameManager::sInstance = NULL;

//create instance of GameManager
GameManager* GameManager::Instance() {

	if (sInstance == NULL) {
		sInstance = new GameManager();
	}

	return sInstance;
}

//free GameManager
void GameManager::Release() {

	delete sInstance;
	sInstance = NULL;
}

//constructor
GameManager::GameManager() {

	srand(time(0));

	mQuit = false;

	mGraphics = Graphics::Instance();

	if (!Graphics::Initialized()) {
		mQuit == true;
	}

	mAssetMgr = AssetManager::Instance();

	mInputMgr = InputManager::Instance();

	mAudioMgr = AudioManager::Instance();

	mTimer = Timer::Instance();

	mScreenMgr = ScreenManager::Instance();
}

//destructor
GameManager::~GameManager() {

	ScreenManager::Release();
	mScreenMgr = NULL;

	AudioManager::Release();
	mAudioMgr = NULL;

	AssetManager::Release();
	mAssetMgr = NULL;

	Graphics::Release();
	mGraphics = NULL;

	InputManager::Release();
	mInputMgr = NULL;

	Timer::Release();
	mTimer = NULL;
}

//early update
void GameManager::EarlyUpdate() {

	mInputMgr->Update();
}

//update
void GameManager::Update() {

	mScreenMgr->Update();

}

//render
void GameManager::Render() {

	mGraphics->ClearBackBuffer();

	mScreenMgr->Render();

	mGraphics->Render();
}

//late update
void GameManager::LateUpdate() {

	mInputMgr->UpdatePrevInput();
	mTimer->Reset();
}

//run main game loop
void GameManager::Run() {

	while (!mQuit) {

		mTimer->Update();

		while (SDL_PollEvent(&mEvents) != 0) {

			if (mEvents.type == SDL_QUIT) {
				mQuit = true;
			}
		}

		if (mTimer->DeltaTime() >= (1.0f / FRAME_RATE)) {

			EarlyUpdate();
			Update();
			LateUpdate();
			Render();
		}
	}
}