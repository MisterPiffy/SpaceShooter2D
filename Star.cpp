#include "Star.h"

bool Star::sScroll = false;

void Star::Scroll(bool b) {

	sScroll = b;
}

Star::Star(int layer)
	:Texture("background/star.png", 0, 0, 4, 4)
{
	mTimer = Timer::Instance();

	int startcolor = rand() % 4;

	mClippedRect.x = startcolor * 32;

	Pos(Vector2(rand() % Graphics::Instance()->SCREEN_WIDTH, rand() % Graphics::Instance()->SCREEN_HEIGHT));

	//set flicker speed
	mFlickerTimer = 0.0f;
	mFlickerSpeed = 0.15f + ((float)rand() / RAND_MAX) * 0.45f;

	float invLayer = 1.0f / layer;
	Scale(VEC2_ONE * invLayer);

	mScrollSpeed = 4.0f / layer;
}

//destroy
Star::~Star() {

	mTimer = NULL;
}

//move stars
void Star::ScrollStar() {

	Translate(VEC2_UP*mScrollSpeed, world);

	Vector2 pos = Pos(local);

	if (pos.y > Graphics::Instance()->SCREEN_HEIGHT) {

		pos.y = 0.0f;
		pos.x = rand() % Graphics::Instance()->SCREEN_WIDTH;
		Pos(pos);
	}
}

void Star::Update() {

	mFlickerTimer += mTimer->DeltaTime();
	if (mFlickerTimer >= mFlickerSpeed) {

		mVisible = !mVisible;
		mFlickerTimer = 0.0f;
	}

	if (sScroll) {
		ScrollStar();
	}
}

void Star::Render() {

	if (mVisible) {
		Texture::Render();
	}
}