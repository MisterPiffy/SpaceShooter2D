#ifndef _ANIMATEDTEXTURE_H
#define _ANIMATEDTEXTURE_H
#include "Timer.h"
#include "Texture.h"

class AnimatedTexture : public Texture {

public:

	enum WRAP_MODE {once = 0, loop = 1};
	enum ANIM_DIR {horizontal = 0, vertical = 1};

private:

	Timer* mTimer;

	int mStartX, mStartY;

	float mAnimTimer;
	float mAnimSpeed;
	float mTimePerFrame;

	int mFrameCount;

	WRAP_MODE mWrapMode;
	ANIM_DIR mAnimDir;

	bool mAnimDone;

public:

	AnimatedTexture(std::string fileName, int x, int y, int w, int h, int frameCount, float animSpeed, ANIM_DIR animDir);
	~AnimatedTexture();

	void WrapMode(WRAP_MODE mode);

	void ResetAnimation();

	bool IsAnimating();

	void Update();
};


#endif // !_ANIMATEDTEXTURE_H
