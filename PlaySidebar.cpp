#include "PlaySideBar.h"

//constructor
PlaySideBar::PlaySideBar() {

	mTimer = Timer::Instance();
	mAudio = AudioManager::Instance();

	mBackground = new Texture("background/sidebar.png");
	mBackground->Parent(this);
	mBackground->Pos(Vector2(45.0f, 380.f));
	mBackground->Scale(Vector2(3.0f, 10.0f));

	mHighLabel = new Texture("HIGH", "fonts/cour.ttf", 32, { 150, 0, 0 });
	mHighLabel->Parent(this);
	mHighLabel->Pos(Vector2(-25.0f, 0.0f));

	mScoreLabel = new Texture("SCORE", "fonts/cour.ttf", 32, { 150, 0, 0 });
	mScoreLabel->Parent(this);
	mScoreLabel->Pos(Vector2(25.0f, 32.0f));

	mHighScoreboard = new Scoreboard();
	mHighScoreboard->Parent(this);
	mHighScoreboard->Pos(Vector2(90.0f, 64.0f));

	mPlayerOneLabel = new Texture("1UP SCORE", "fonts/cour.ttf", 32, { 150, 0, 0 });
	mPlayerOneLabel->Parent(this);
	mPlayerOneLabel->Pos(Vector2(25.0f, 160.0f));

	mPlayerOneScore = new Scoreboard();
	mPlayerOneScore->Parent(this);
	mPlayerOneScore->Pos(Vector2(90.0f, 192.0f));

	mLevelLabel = new Texture("LEVEL", "fonts/cour.ttf", 32, { 150, 0, 0 });
	mLevelLabel->Parent(this);
	mLevelLabel->Pos(Vector2(25.0f, 650.0f));

	mLevel = new Scoreboard();
	mLevel->Parent(this);
	mLevel->Pos(Vector2(55.0f, 682.0f));

	mLives = new Texture("LIVES", "fonts/cour.ttf", 32, { 150, 0, 0 });
	mLives->Parent(this);
	mLives->Pos(Vector2(25.0f, 550.0f));

	mTotalLivesLabel = new Scoreboard();
	mTotalLivesLabel->Parent(this);
	mTotalLivesLabel->Pos(Vector2(55.0f, 582.0f));
}

//destructor
PlaySideBar::~PlaySideBar() {
	
	mTimer = NULL;
	mAudio = NULL;

	delete mBackground;
	mBackground = NULL;

	delete mHighLabel;
	mHighLabel = NULL;
	delete mScoreLabel;
	mScoreLabel = NULL;
	delete mHighScoreboard;
	mHighScoreboard = NULL;

	delete mPlayerOneLabel;
	mPlayerOneLabel = NULL;

	delete mPlayerOneScore;
	mPlayerOneScore = NULL;

	delete mTotalLivesLabel;
	mTotalLivesLabel = NULL;

	delete mLevelLabel;
	mLevelLabel = NULL;

	delete mLevel;
	mLevel = NULL;

	delete mLives;
	mLives = NULL;
}

void PlaySideBar::SetHighScore(int score) {

	mHighScoreboard->Score(score);
}

void PlaySideBar::SetPlayerScore(int score) {

	mPlayerOneScore->Score(score);
}

void PlaySideBar::SetLives(int lives) {

	mTotalLivesLabel->Score(lives);
}

void PlaySideBar::SetLevel(int level) {

	mLevel->Score(level);
}

//update
void PlaySideBar::Update() {

}

//render
void PlaySideBar::Render() {

	mBackground->Render();

	mHighLabel->Render();
	mScoreLabel->Render();
	mHighScoreboard->Render();

	mPlayerOneLabel->Render();
	mPlayerOneScore->Render();

	mLives->Render();
	mTotalLivesLabel->Render();

	mLevelLabel->Render();
	mLevel->Render();
}