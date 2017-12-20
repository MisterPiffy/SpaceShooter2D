#include "CaptureBeam.h"

//constructor
CaptureBeam::CaptureBeam()
	: AnimatedTexture("ships/YellowBeamFullLarge.png", 0, 0, 100, 144, 3, 0.5f, horizontal)
{
	mTotalCaptureTime = 6.0f;
	ResetAnimation();
}

//destructor
CaptureBeam::~CaptureBeam() {


}

void CaptureBeam::ResetAnimation() {

	AnimatedTexture::ResetAnimation();
	mCaptureTimer = 0.0f;
	mClippedRect.h = 0;
}

void CaptureBeam::RunAnimation() {

	mCaptureTimer += mTimer->DeltaTime();
	if (mCaptureTimer >= mTotalCaptureTime) {
		mAnimDone = true;
	}
	else {

		mAnimTimer += mTimer->DeltaTime();

		if (mAnimTimer >= mAnimSpeed) {
			mAnimTimer -= mAnimSpeed;
		}

		mClippedRect.x = (int)(mAnimTimer / mTimePerFrame) * mWidth;

		if (mCaptureTimer < 2.0f) {

			float temp = (mCaptureTimer * 3.5f);
			mClippedRect.h = ((temp / 7.0f) * mHeight);
		}
		else if (mCaptureTimer > mTotalCaptureTime - 2.0f) {

			float temp = ((mTotalCaptureTime - mCaptureTimer) * 3.5f);
			mClippedRect.h = ((temp / 7.0f) * mHeight);
		}
		else {

			mClippedRect.h = mHeight;
		}
	}
}

void CaptureBeam::Render() {

	Vector2 pos = Pos(world);
	Vector2 scale = Scale(world);
	mRenderRect.x = (int)(pos.x - mWidth*scale.x*0.5f);
	mRenderRect.y = (int)(pos.y - mHeight*scale.y*0.5f);
	mRenderRect.w = (int)(mWidth*scale.x);
	mRenderRect.h = mClippedRect.h;

	mGraphics->DrawTexture(mTex, (mClipped) ? &mClippedRect : NULL, &mRenderRect, Rotation(world));
}