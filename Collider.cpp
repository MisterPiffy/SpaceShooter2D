#include "Collider.h"

//constructor
Collider::Collider(ColliderType type)
	: mType(type)
{
	mDebugTexture = nullptr;
}

//destructor
Collider::~Collider() {

	if (mDebugTexture) {

		delete mDebugTexture;
		mDebugTexture = nullptr;
	}
}

//set texture
void Collider::SetDebugTexture(Texture* texture) {

	if (mDebugTexture) {
		delete mDebugTexture;
	}
	mDebugTexture = texture;
	mDebugTexture->Parent(this);
}

Collider::ColliderType Collider::GetType() {

	return mType;
}

//render
void Collider::Render() {

	if (DEBUG_COLLIDERS) {

		mDebugTexture->Render();
	}
}