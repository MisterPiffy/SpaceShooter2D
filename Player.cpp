#include "Player.h"

//constructor
Player::Player() {

	mTimer = Timer::Instance();
	mInput = InputManager::Instance();
	mAudio = AudioManager::Instance();

	mVisible = false;
	mAnimating = false;

	mScore = 0;
	mLives = 2;

	mShip = new Texture("ships/Marksman.png");
	mShip->Parent(this);
	mShip->Pos(VEC2_ZERO);

	mMoveSpeed = 300.0f;
	mMoveBounds = Vector2(30.0f, 820.0f);

	mDeathAnimation = new AnimatedTexture("ships/Explosion.png", 0, 0, 64, 64, 6, 1.0f, AnimatedTexture::horizontal);
	mDeathAnimation->Parent(this);
	mDeathAnimation->Pos(VEC2_ZERO);
	mDeathAnimation->WrapMode(AnimatedTexture::once);

	for (int i = 0; i < MAX_BULLETS; i++) {

		mBullets[i] = new Bullet();
	}
}

//destructor
Player::~Player() {
	
	mTimer = NULL;
	mInput = NULL;
	mAudio = NULL;

	delete mShip;
	mShip = NULL;

	delete mDeathAnimation;
	mDeathAnimation = NULL;

	for (int i = 0; i < MAX_BULLETS; i++) {

		delete mBullets[i];
		mBullets[i] = NULL;
	}
}

//movement
void Player::HandleMovement() {

	if (mInput->KeyDown(SDL_SCANCODE_RIGHT)) {

		Translate(VEC2_RIGHT*mMoveSpeed*mTimer->DeltaTime(), world);
	}
	else if (mInput->KeyDown(SDL_SCANCODE_LEFT)) {

		Translate(-VEC2_RIGHT*mMoveSpeed*mTimer->DeltaTime(), world);
	}
	else if (mInput->KeyDown(SDL_SCANCODE_DOWN)) {

		Translate(VEC2_UP*mMoveSpeed*mTimer->DeltaTime(), world);
	}
	else if (mInput->KeyDown(SDL_SCANCODE_UP)) {

		Translate(-VEC2_UP*mMoveSpeed*mTimer->DeltaTime(), world);
	}

	Vector2 pos = Pos(local);
	if (pos.x < mMoveBounds.x) {
		pos.x = mMoveBounds.x;
	}
	else if (pos.x > mMoveBounds.y) {
		pos.x = mMoveBounds.y;
	}
	else if (pos.y > 800) {
		pos.y = 800;
	}
	else if (pos.y < 400) {
		pos.y = 400;
	}

	Pos(pos);
}

//handle firing
void Player::HandleFiring() {

	if (mInput->KeyPressed(SDL_SCANCODE_SPACE)) {

		for (int i = 0; i < MAX_BULLETS; i++) {

			if (!mBullets[i]->Active()) {

				mBullets[i]->Fire(Pos());
				mAudio->PlaySFX("audio/playerFire.wav");
				break;
			}
		}
	}
}

//set visibility
void Player::Visible(bool visible) {

	mVisible = visible;
}

//check to see if animating
bool Player::IsAnimating() {

	return mAnimating;
}

//get score
int Player::Score() {

	return mScore;
}

//get lives
int Player::Lives() {

	return mLives;
}

//add score
void Player::AddScore(int change) {

	mScore += change;
}

//if collision was detected
void Player::WasHit() {

	mLives--;
	mDeathAnimation->ResetAnimation();
	mAnimating = true;
	mAudio->PlaySFX("audio/Explosion.wav");
}

//update 
void Player::Update() {

	if (mAnimating) {

		mDeathAnimation->Update();
		mAnimating = mDeathAnimation->IsAnimating();
	}
	else {

		if (Active()) {

			HandleMovement();
			HandleFiring();
		}
	}

	for (int i = 0; i < MAX_BULLETS; i++) {

		mBullets[i]->Update();
	}
}

//render player
void Player::Render() {

	if (mVisible) {

		if (mAnimating) {

			mDeathAnimation->Render();
		}
		else if(!mAnimating || mLives > 0){

			mShip->Render();
		}
	}
	for (int i = 0; i < MAX_BULLETS; i++) {

		mBullets[i]->Render();
	}
}