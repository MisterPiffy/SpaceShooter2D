#include "GameEntity.h"

//constructor
GameEntity::GameEntity(float x, float y) {

	mPos.x = x;
	mPos.y = y;

	mRotation = 0.0f;

	mActive = true;

	mParent = NULL;

	mScale = VEC2_ONE;
}

//destructor
GameEntity::~GameEntity() {

	mParent = NULL;
}

//set pos
void GameEntity::Pos(Vector2 pos) {

	mPos = pos;
}

//get pos
Vector2 GameEntity::Pos(SPACE space) {

	if (space == local || mParent == NULL) {
		return mPos;
	}

	Vector2 parentScale = mParent->Scale(world);
	Vector2 rotPos = RotateVector(Vector2(mPos.x * parentScale.x, mPos.y * parentScale.y), mParent->Rotation(local));

	return mParent->Pos(world) + rotPos;
}

//set rot
void GameEntity::Rotation(float r) {

	mRotation = r;

	if (mRotation > 360.0f) {
		mRotation -= 360.0;
	}
	if (mRotation < 0.0f) {
		mRotation += 360.0;
	}
}

//get rot
float GameEntity::Rotation(SPACE space) {

	if (space == local || mParent == NULL) {
		return mRotation;
	}
	else {
		return mParent->Rotation(world) + mRotation;
	}
}

//set scale
void GameEntity::Scale(Vector2 scale) {

	mScale = scale;
}

//get scale
Vector2 GameEntity::Scale(SPACE space) {

	if (space == local || mParent == NULL) {
		return mScale;
	}

	Vector2 scale = mParent->Scale(world);
	scale.x *= mScale.x;
	scale.y *= mScale.y;

	return scale;
}

//set active
void GameEntity::Active(bool active) {

	mActive = active;
}

//get active
bool GameEntity::Active() {

	return mActive;
}

//set parent
void GameEntity::Parent(GameEntity* parent) {

	if (parent == NULL) {

		mPos = Pos(world);
		mRotation = Rotation(world);
		mScale = Scale(world);
	}
	else {
		
		if (mParent != NULL) {
			Parent(NULL);
		}

		Vector2 parentScale = parent->Scale(world);

		mPos = RotateVector(Pos(world) - parent->Pos(world), -parent->Rotation(world));
		mPos.x /= parentScale.x;
		mPos.y /= parentScale.y;

		mRotation -= parent->Rotation(world);

		mScale = Vector2(mScale.x / parentScale.x, mScale.y / parentScale.y);
	}

	mParent = parent;
}

//get parent
GameEntity* GameEntity::Parent() {

	return mParent;
}

//move
void GameEntity::Translate(Vector2 vec, SPACE space) {

	if (space == world) {

		mPos += vec;
	}
	else {

		mPos += RotateVector(vec, Rotation());
	}
}

//rotate
void GameEntity::Rotate(float amount) {
	mRotation += amount;
}

//update entities
void GameEntity::Update() {

}

//render entity
void GameEntity::Render() {

}