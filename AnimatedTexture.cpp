#include "AnimatedTexture.h"

//constructor
AnimatedTexture::AnimatedTexture(std::string fileName, int x, int y, int w, int h, int frameCount, float animSpeed, ANIM_DIR animDir)
	: Texture(fileName, x, y, w, h)
{
	mTimer = Timer::Instance();

	mStartX = x;
	mStartY = x;

	mFrameCount = frameCount;
	mAnimSpeed = animSpeed;
	mTimePerFrame = mAnimSpeed / mFrameCount;
	mAnimTimer = 0.0f;

	mAnimDir = animDir;

	mAnimDone = false;

	mWrapMode = loop;
}

//destructor
AnimatedTexture::~AnimatedTexture() {

}

void AnimatedTexture::RunAnimation() {

	mAnimTimer += mTimer->DeltaTime();

	if (mAnimTimer >= mAnimSpeed) {

		if (mWrapMode == loop) {

			mAnimTimer -= mAnimSpeed;
		}
		else {

			mAnimDone = true;
			mAnimTimer = mAnimSpeed - mTimePerFrame;
		}
	}

	if (mAnimDir == horizontal) {

		mClippedRect.x = mStartX + (int)(mAnimTimer / mTimePerFrame) * mWidth;
	}
	else {

		mClippedRect.y = mStartY + (int)(mAnimTimer / mTimePerFrame) * mHeight;
	}
}

//set wrap mode
void AnimatedTexture::WrapMode(WRAP_MODE mode) {

	mWrapMode = mode;
}

void AnimatedTexture::ResetAnimation() {

	mAnimTimer = 0.0f;
	mAnimDone = false;
}

bool AnimatedTexture::IsAnimating() {

	return !mAnimDone;
}

//update
void AnimatedTexture::Update() {

	if (!mAnimDone) {

		RunAnimation();
	}
}