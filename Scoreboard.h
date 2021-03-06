#ifndef _SCOREBOARD_H
#define _SCOREBOARD_H
#include "Texture.h"
#include <vector>

class Scoreboard : public GameEntity {

private:

	std::vector<Texture*> mScore;

public:

	Scoreboard();
	~Scoreboard();

	void Score(int score);

	void Render();

private:

	void ClearBoard();

};

#endif // !_SCOREBOARD_H