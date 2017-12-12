#ifndef _STAR_H
#define _STAR_H
#include "Texture.h"
#include "Timer.h"

class Star : public Texture {

private:

	static bool sScroll;

	Timer* mTimer;

	bool mVisible;
	float mFlickerTimer;
	float mFlickerSpeed;

	float mScrollSpeed;

public:

	static void Scroll(bool b);

	Star(int layer);
	~Star();

	void Update();
	void Render();

private:

	void ScrollStar();
};

#endif // !_STAR_H