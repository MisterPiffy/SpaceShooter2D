#ifndef _PLAYSIDEBAR_H
#define _PLAYSIDEBAR_H
#include "Timer.h"
#include "Scoreboard.h"
#include "AudioManager.h"

class PlaySideBar : public GameEntity {

private:

	Timer* mTimer;
	AudioManager* mAudio;

	Texture* mBackground;

	Texture* mHighLabel;
	Texture* mScoreLabel;
	Scoreboard* mHighScoreboard;

	Texture* mPlayerOneLabel;
	Scoreboard* mPlayerOneScore;

	int mTotalLives;
	Scoreboard* mTotalLivesLabel;
	Texture* mLives;

	Texture* mLevelLabel;
	Scoreboard* mLevel;

public:

	PlaySideBar();
	~PlaySideBar();

	void SetHighScore(int score);
	void SetPlayerScore(int score);
	void SetLives(int lives);
	void SetLevel(int level);
	void Update();

	void Render();
};

#endif // !_PLAYSIDEBAR_H