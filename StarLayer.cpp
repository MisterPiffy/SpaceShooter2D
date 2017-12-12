#include "StarLayer.h"

//construct
StarLayer::StarLayer(int layer) {

	for (int i = 0; i < STAR_COUNT; i++) {

		mStars[i] = new Star(layer);
	}
}

//destruct
StarLayer::~StarLayer() {

	for (int i = 0; i < STAR_COUNT; i++) {

		delete mStars[i];
		mStars[i] = NULL;
	}
}

//update
void StarLayer::Update() {

	for (int i = 0; i < STAR_COUNT; i++) {
		mStars[i]->Update();
	}
}

//render
void StarLayer::Render() {

	for (int i = 0; i < STAR_COUNT; i++) {
		mStars[i]->Render();
	}
}