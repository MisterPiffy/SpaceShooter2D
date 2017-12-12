#ifndef _STARTSCREEN_H
#define _STARTSCREEN_H
#include "AnimatedTexture.h"
#include "InputManager.h"
#include "Scoreboard.h"

class StartScreen : public GameEntity {

private:

	InputManager* mInput;

	//menu entities
	GameEntity* mTopBar;
	Texture* mStartGame;
	Texture* mPlayerOne;
	Texture* mQuit;
	Texture* mHiScore;
	AnimatedTexture* mCursor;
	Scoreboard* mPlayerOneScore;
	Scoreboard* mTopScore;

	Vector2 mCursorStartPos;
	Vector2 mCursorOffset;

	int mSelected;

public:

	StartScreen();
	~StartScreen();

	int Selected();
	void ChangeSelection(int change);

	void Update();

	void Render();

};

#endif // !_STARTSCREEN_H
