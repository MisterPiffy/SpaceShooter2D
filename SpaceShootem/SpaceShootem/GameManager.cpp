#include "GameManager.h"

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

	mQuit = false;

	mGraphics = Graphics::Instance();

	if (!Graphics::Initialized()) {
		mQuit == true;
	}

	mAssetMgr = AssetManager::Instance();

	mInputMgr = InputManager::Instance();

	mAudioMgr = AudioManager::Instance();

	mTimer = Timer::Instance();

	mTex = new Texture("ships/Marksman.png");
	mTex->Pos(Vector2(Graphics::SCREEN_WIDTH*0.5f, Graphics::SCREEN_HEIGHT*0.5f));
}

//destructor
GameManager::~GameManager() {

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

	delete mTex;
	mTex = NULL;
}

//early update
void GameManager::EarlyUpdate() {

	mInputMgr->Update();
}

//update
void GameManager::Update() {

	//moving player ship over time
	if (mInputMgr->KeyDown(SDL_SCANCODE_W)) {

		mTex->Translate(Vector2(0.0f, -80.0f) * mTimer->DeltaTime());
	}
	else if (mInputMgr->KeyDown(SDL_SCANCODE_S)) {

		mTex->Translate(Vector2(0.0f, 80.0f) * mTimer->DeltaTime());
	}
	else if (mInputMgr->KeyDown(SDL_SCANCODE_A)) {

		mTex->Translate(Vector2(-80.0f, 0.0f) * mTimer->DeltaTime());
	}
	else if (mInputMgr->KeyDown(SDL_SCANCODE_D)) {

		mTex->Translate(Vector2(80.0f, 0.0f) * mTimer->DeltaTime());
	}
	else if (mInputMgr->KeyDown(SDL_SCANCODE_SPACE)) {

		mAudioMgr->PlaySFX("audio/PlayerFire.wav");
	}
}

//render
void GameManager::Render() {

	mGraphics->ClearBackBuffer();

	mTex->Render();

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