#include "StartScreen.h"

//constructor
StartScreen::StartScreen() {

	mInput = InputManager::Instance();

	mTopBar = new GameEntity(512.0f, 40.0f);
	mStartGame = new Texture("Start Game", "fonts/cour.ttf", 32, { 0, 200, 0 });
	mPlayerOne = new Texture("Player One", "fonts/cour.ttf", 32, { 0, 200, 0 });
	mHiScore = new Texture("High Score", "fonts/cour.ttf", 32, { 0, 200, 0 });
	mQuit = new Texture("Quit", "fonts/cour.ttf", 32, { 0, 200, 0 });
	mCursor = new AnimatedTexture("background/Pointer.png", 0, 0, 32, 32, 10, 1, AnimatedTexture::horizontal);
	mPlayerOneScore = new Scoreboard();
	mTopScore = new Scoreboard();

	mStartGame->Parent(mTopBar);
	mPlayerOne->Parent(mTopBar);
	mHiScore->Parent(mTopBar);
	mQuit->Parent(mTopBar);
	mCursor->Parent(mTopBar);
	mPlayerOneScore->Parent(mTopBar);
	mTopScore->Parent(mTopBar);

	mPlayerOne->Pos(Vector2(-350.0f, 50.0f));
	mStartGame->Pos(Vector2(0.0f, 300.0f));
	mHiScore->Pos(Vector2(0.0f, 450.0f));
	mQuit->Pos(Vector2(0.0f, 600.0f));
	mCursor->Pos(Vector2(-120.0f, 300.0f));

	mPlayerOneScore->Pos(Vector2(-350.0f, 85.0f));
	mTopScore->Pos(Vector2(60.0f, 500.0f));

	mCursorStartPos = mCursor->Pos(local);
	mCursorOffset = Vector2(0.0f, 150.0f);
	mSelected = 0;

	mTopScore->Score(30000);

	mTopBar->Parent(this);
}

//destructor
StartScreen::~StartScreen() {

	delete mTopBar;
	mTopBar = NULL;
	delete mStartGame;
	mStartGame = NULL;
	delete mPlayerOne;
	mPlayerOne = NULL;
	delete mHiScore;
	mHiScore = NULL;
	delete mQuit;
	mQuit = NULL;
	delete mCursor;
	mCursor = NULL;
	delete mPlayerOneScore;
	mPlayerOneScore = NULL;
	delete mTopScore;
	mTopScore = NULL;
}

int StartScreen::Selected() {

	return mSelected;
}

void StartScreen::ChangeSelection(int change) {

	mSelected += change;

	if (mSelected < 0) {
		mSelected = 2;
	}
	else if (mSelected > 2) {
		mSelected = 0;
	}

	mCursor->Pos(mCursorStartPos + mCursorOffset * mSelected);
}

//update
void StartScreen::Update() {

	mCursor->Update();

	if (mInput->KeyPressed(SDL_SCANCODE_DOWN)) {
		ChangeSelection(1);
	}
	else if (mInput->KeyPressed(SDL_SCANCODE_UP)) {
		ChangeSelection(-1);
	}
}

//render
void StartScreen::Render() {

	mStartGame->Render();
	mPlayerOne->Render();
	mHiScore->Render();
	mQuit->Render();
	mCursor->Render();
	mPlayerOneScore->Render();
	mTopScore->Render();
}
