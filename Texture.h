#ifndef _TEXTURE_H
#define _TEXTURE_H
#include "GameEntity.h"
#include "AssetManager.h"

class Texture : public GameEntity {

protected:

	SDL_Texture* mTex;

	Graphics* mGraphics;

	int mHeight, mWidth;

	bool mClipped;
	SDL_Rect mRenderRect;
	SDL_Rect mClippedRect;

public:

	Texture(std::string fileName);
	Texture(std::string fileName, int x, int y, int w, int h);
	Texture(std::string text, std::string fontpath, int size, SDL_Color color);
	~Texture();

	Vector2 ScaledDimensions();

	virtual void Render();
};

#endif