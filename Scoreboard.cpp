#include "Scoreboard.h"

//constructor
Scoreboard::Scoreboard() {

	Score(0);
}

//destructor
Scoreboard::~Scoreboard() {

	ClearBoard();
}

//clearing board
void Scoreboard::ClearBoard() {

	for (int i = 0; i < mScore.size(); i++) {
		
		delete mScore[i];
		mScore[i] = NULL;
	}
	
	mScore.clear();
}

//scoring
void Scoreboard::Score(int score) {

	ClearBoard();

	if (score == 0) {

		for (int i = 0; i < 2; i++) {

			mScore.push_back(new Texture("0", "fonts/cour.ttf", 32, { 230, 230, 230 }));
			mScore[i]->Parent(this);
			mScore[i]->Pos(Vector2(-32.0f * i, 0.0f));
		}
	}
	else {

		std::string str = std::to_string(score);
		int lastIndex = str.length() - 1;

		for (int i = 0; i <= lastIndex; i++) {

			mScore.push_back(new Texture(str.substr(i, 1), "fonts/cour.ttf", 32, { 230, 230, 230 }));
			mScore[i]->Parent(this);
			mScore[i]->Pos(Vector2(-32.0f*(lastIndex - i), 0.0f));
		}
	}
}

//render
void Scoreboard::Render() {

	for (int i = 0; i < mScore.size(); i++) {

		mScore[i]->Render();
	}
}