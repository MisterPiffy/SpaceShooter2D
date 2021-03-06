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

//set wrap mode
void AnimatedTexture::WrapMode(WRAP_MODE mode) {

	mWrapMode = mode;
}

//update
void AnimatedTexture::Update() {

	if (!mAnimDone) {

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
}