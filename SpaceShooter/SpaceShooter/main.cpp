#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <sstream>

//Screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 800;

const int BUTTON_WIDTH = 200;
const int BUTTON_HEIGHT = 80;
const int TOTAL_BUTTONS = 3;

//build an array of projectiles add to it each time user fires
const int maxProj = 25;

bool shooting = false;

//sound effects
Mix_Chunk *playerFire = NULL;
Mix_Chunk *playerHit = NULL;

//player
class Player
{
public:
	static const int PLAYER_WIDTH = 45;
	static const int PLAYER_HEIGHT = 45;

	static const int PLAYER_VEL = 5;

	Player();

	void handleEvent(SDL_Event&e);

	void move();

	void render();

	void shoot();

	int posX, posY;

	int projPosX, projPosY;

	int velX, velY;
};

Player::Player()
{
	posX = 500;
	posY = 600;

	projPosX = posX;
	projPosY = posY;

	velX = 0;
	velY = 0;
}

void Player::handleEvent(SDL_Event &e)
{
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: velY -= PLAYER_VEL; break;
		case SDLK_DOWN: velY += PLAYER_VEL; break;
		case SDLK_LEFT: velX -= PLAYER_VEL; break;
		case SDLK_RIGHT: velX += PLAYER_VEL; break;
		case SDLK_SPACE: shoot(); break;
		}
	}
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: velY += PLAYER_VEL; break;
		case SDLK_DOWN: velY -= PLAYER_VEL; break;
		case SDLK_LEFT: velX += PLAYER_VEL; break;
		case SDLK_RIGHT: velX -= PLAYER_VEL; break;
		}
	}
}

void Player::move()
{
	//left or right
	posX += velX;
	projPosX += velX;

	if ((posX < 0) || (posX + PLAYER_WIDTH > SCREEN_WIDTH))
	{
		posX -= velX;
		projPosX -= velX;
	}

	//up or down
	posY += velY;
	projPosY += velY;

	if ((posY < 0) || (posY + PLAYER_HEIGHT > SCREEN_HEIGHT) || (posY + PLAYER_HEIGHT < 400))
	{
		posY -= velY;
		projPosY -= velY;
	}
}

Player player;

class Projectile 
{
public:
	static const int SHOT_VEL = 10;

	Projectile();

	void move();

	void renderShot();

	int projectileX, projectileY;
};

Projectile::Projectile()
{
	projectileX = player.projPosX;
	projectileY = player.projPosY;
}

void Projectile::move()
{
	projectileY -= SHOT_VEL;
}

Projectile proj;

//custom timer
class Timer
{
public:
	Timer();

	void start();
	void stop();
	void pause();
	void unpause();

	Uint32 getTicks();

	bool isStarted();
	bool isPaused();

private:
	Uint32 startTicks;

	Uint32 pausedTicks;

	bool paused;
	bool started;
};

Timer::Timer()
{
	startTicks = 0;
	pausedTicks = 0;

	paused = false;
	started = false;
}

void Timer::start()
{
	started = true;
	paused = false;

	startTicks = SDL_GetTicks();
	pausedTicks = 0;
}

void Timer::stop()
{
	started = false;
	paused = false;

	startTicks = 0;
	pausedTicks = 0;
}

void Timer::pause()
{
	if (started && !paused)
	{
		paused = true;

		pausedTicks = SDL_GetTicks() - startTicks;
		startTicks = 0;
	}
}

void Timer::unpause()
{
	if (started && paused)
	{
		paused = false;

		startTicks = SDL_GetTicks() - pausedTicks;

		pausedTicks = 0;
	}
}

Uint32 Timer::getTicks()
{
	//timer time
	Uint32 time = 0;

	if (started)
	{
		if (paused)
		{
			time = pausedTicks;
		}
		else
		{
			time = SDL_GetTicks() - startTicks;
		}
	}
	return time;
}

bool Timer::isStarted()
{
	return started;
}

bool Timer::isPaused()
{
	return paused && started;
}

//texture wrapper
class Texture
{
public:
	Texture();

	~Texture();

	//loads image at path
	bool loadFromFile(std::string path);

	//create image from font string
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);

	//deallocate texture
	void free();

	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	void setBlendMode(SDL_BlendMode blending);

	void setAlpha(Uint8 alpha);

	void render(int x, int y, SDL_Rect *clip = NULL, double angle = 0.0, SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//image dimensions
	int getWidth();
	int getHeight();

private:
	//actual texture
	SDL_Texture *mTexture;

	//image dimensions
	int mWidth;
	int mHeight;
};

class LButton
{
public:
	LButton();

	void setPosition(int x, int y);

	void handleEvent(SDL_Event *e);

	void renderStart();

	void renderQuit();

	void renderScore();

private:
	SDL_Point mPosition;
};

//start SDL create window
bool init();

//main menu
void mainMenu();

//Load media
bool loadMedia();

//free resources
void end();

//load image
SDL_Texture *loadTexture(std::string path);

//main loop flag
bool quit = false;
bool menu = true;

SDL_Event e;

SDL_Window *gameWindow = NULL;
SDL_Renderer *gameRenderer = NULL;
SDL_Texture *gameTexture = NULL;
SDL_Texture *menuTexture = NULL;

TTF_Font *font = NULL;
Texture textTexture;

SDL_Rect startRect;
SDL_Rect scoreRect;
SDL_Rect quitRect;
Texture startTexture;
Texture scoreTexture;
Texture quitTexture;

Texture playerTexture;
Texture playerFireTexture;

LButton gButtons[TOTAL_BUTTONS];

Timer timer;

std::stringstream timeText;

int projNum = 0;

//music
Mix_Music *music = NULL;

bool init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//create window
		gameWindow = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gameWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//renderer for window
			gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gameRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//initial color
				SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				
				//png loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags)&imgFlags))
				{
					printf("SDL_image could not be initialized! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				//ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not be initialized! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}

				//SDL_mixer
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					printf("SDL_mixer could not initialize! SDL_mixer Error: %\n", Mix_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

void mainMenu()
{
	SDL_Color wTextColor = { 255, 255, 255, 255 };

	//load menu background
	menuTexture = loadTexture("images/backgrounds/space1.png");

	startTexture.loadFromRenderedText("Start", wTextColor);
	scoreTexture.loadFromRenderedText("Scores", wTextColor);
	quitTexture.loadFromRenderedText("Quit", wTextColor);

	//place button
	gButtons[0].setPosition(500, 300);
	gButtons[1].setPosition(500, 500);
	gButtons[2].setPosition(500, 700);
}

bool loadMedia()
{
	bool success = true;

	//open font
	font = TTF_OpenFont("C:/Windows/Fonts/Centaur.ttf", 36);
	if (font == NULL)
	{
		printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}

	//load music
	music = Mix_LoadMUS("audio/spaceChase.wav");
	if (music == NULL)
	{
		printf("Failed to load music! SDL_mixer Error: %\n", Mix_GetError());
		success = false;
	}

	//load sound effects
	playerFire = Mix_LoadWAV("audio/playerFire.wav");
	if (playerFire == NULL)
	{
		printf("Failed to load sound effect! SDL_mixer Error: %\n", Mix_GetError());
		success = false;
	}

	playerHit = Mix_LoadWAV("audio/playerHit.wav");
	if (playerHit == NULL)
	{
		printf("Failed to load sound effect! SDL_mixer Error: %\n", Mix_GetError());
		success = false;
	}

	gameTexture = loadTexture("images/backgrounds/space1.png");
	if (gameTexture == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", "images/backgrounds/space1.png", SDL_GetError());
		success = false;
	}

	if (!playerTexture.loadFromFile("images/ships/Marksman.png"))
	{
		printf("Failed to load player texture!\n");
		success = false;
	}

	if (!playerFireTexture.loadFromFile("images/ships/RedFire.png"))
	{
		printf("Failed to load texture!\n");
		success = false;
	}

	return success;
}

void end()
{
	textTexture.free();
	playerTexture.free();
	playerFireTexture.free();

	TTF_CloseFont(font);
	font = NULL;

	SDL_DestroyTexture(gameTexture);
	gameTexture = NULL;

	Mix_FreeChunk(playerFire);
	Mix_FreeChunk(playerHit);
	playerFire = NULL;
	playerHit = NULL;

	Mix_FreeMusic(music);
	music = NULL;

	//Destroy window
	SDL_DestroyRenderer(gameRenderer);
	SDL_DestroyWindow(gameWindow);
	gameRenderer = NULL;
	gameWindow = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	Mix_Quit();
}

void Player::render()
{
	playerTexture.render(posX, posY);
}

void Projectile::renderShot()
{
	playerFireTexture.render(projectileX, projectileY);
}

void Player::shoot()
{
	shooting = true;
	Mix_PlayChannel(-1, playerFire, 0);
	projNum += 1;
}


LButton::LButton()
{
	mPosition.x = 0;
	mPosition.y = 0;
}

void LButton::setPosition(int x, int y)
{
	mPosition.x = x;
	mPosition.y = y;
}

void LButton::handleEvent(SDL_Event *e)
{
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN)
	{
		//mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if (x < mPosition.x)
		{
			inside = false;
		}
		//Mouse is right of the button
		else if (x > mPosition.x + BUTTON_WIDTH)
		{
			inside = false;
		}
		//Mouse above the button
		else if (y < mPosition.y)
		{
			inside = false;
		}
		//Mouse below the button
		else if (y > mPosition.y + BUTTON_HEIGHT)
		{
			inside = false;
		}
			switch (e->type)
			{
			case SDL_MOUSEMOTION:
				if (inside && (y >= 260 && y <= 340))
				{
					startTexture.setColor(0, 0, 0);
					scoreTexture.setColor(255, 255, 255);
					quitTexture.setColor(255, 255, 255);
				}
				else if (inside && (y >= 460 && y <= 540))
				{
					scoreTexture.setColor(0, 0, 0);
					startTexture.setColor(255, 255, 255);
					quitTexture.setColor(255, 255, 255);
				}
				else if (inside && (y >= 660 && y <= 740))
				{
					quitTexture.setColor(0, 0, 0);
					startTexture.setColor(255, 255, 255);
					scoreTexture.setColor(255, 255, 255);
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (inside && (y >= 260 && y <= 340)) {
					menu = false;
				}
				else if (inside && (y >= 460 && y <= 540))
				{
					//show high scores
				}
				else if (inside && (y >= 660 && y <= 740))
				{
					quit = true;
					end();
				}
				break;
			}
		}
	}

void LButton::renderStart()
{
	scoreTexture.render(mPosition.x, mPosition.y);
}

void LButton::renderQuit()
{
	quitTexture.render(mPosition.x, mPosition.y);
}

void LButton::renderScore()
{
	startTexture.render(mPosition.x, mPosition.y);
}

SDL_Texture *loadTexture(std::string path)
{
	//final image
	SDL_Texture *newTexture = NULL;

	//load image at specified path
	SDL_Surface *loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture
		newTexture = SDL_CreateTextureFromSurface(gameRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

Texture::Texture()
{
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

Texture::~Texture()
{
	free();
}

bool Texture::loadFromFile(std::string path)
{
	free();

	//final texture
	SDL_Texture *newTexture = NULL;

	SDL_Surface *loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//set color
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//creat texture
		newTexture = SDL_CreateTextureFromSurface(gameRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %\n", path.c_str(), SDL_GetError());
		}
		else
		{
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		SDL_FreeSurface(loadedSurface);
	}
	
	mTexture = newTexture;
	return mTexture != NULL;
}

bool Texture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
	free();

	//render text surface
	SDL_Surface *textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %\n", TTF_GetError());
	}
	else
	{
		//create texture
		mTexture = SDL_CreateTextureFromSurface(gameRenderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %\n", SDL_GetError());
		}
		else
		{
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		SDL_FreeSurface(textSurface);
	}

	return mTexture != NULL;
}

void Texture::free()
{
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void Texture::setBlendMode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(mTexture, blending);
}

void Texture::setAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void Texture::render(int x, int y, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip)
{
	//set rendering space then render
	SDL_Rect renderQuad = { x,y,mWidth,mHeight };

	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopyEx(gameRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int Texture::getWidth()
{
	return mWidth;
}

int Texture::getHeight()
{
	return mHeight;
}

int main(int argc, char* args[])
{
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
				bool quit = false;
				SDL_Color wTextColor = { 255, 255, 255, 255 };
				SDL_Event e;
				int score = 0;

				mainMenu();

				//play music
				Mix_PlayMusic(music, -1);

				//main loop
				while (!quit)
				{
					while (menu)
					{
						//Handle events on queue
						while (SDL_PollEvent(&e) != 0)
						{
							//User requests quit
							if (e.type == SDL_QUIT)
							{
								quit = true;
							}

							//Handle button events
							for (int i = 0; i < TOTAL_BUTTONS; ++i)
							{
								gButtons[i].handleEvent(&e);
							}
						}

						SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
						SDL_RenderClear(gameRenderer);

						SDL_RenderCopy(gameRenderer, menuTexture, NULL, NULL);

						gButtons[1].renderStart();
						gButtons[0].renderScore();
						gButtons[2].renderQuit();

						SDL_RenderPresent(gameRenderer);
					}

					//scoring
					std::string scoreString = std::to_string(score);
					textTexture.loadFromRenderedText("Score: " + scoreString, wTextColor);

					//handle events
					while (SDL_PollEvent(&e) != 0)
					{
						player.handleEvent(e);
						//User requests quit
						if (e.type == SDL_QUIT)
						{
							quit = true;
						}
					}

					player.move();
					proj.move();

					//clear screen
					SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
					SDL_RenderClear(gameRenderer);

					//render texture
					SDL_RenderCopy(gameRenderer, gameTexture, NULL, NULL);
					player.render();
					proj.renderShot();

					//Render current frame
					textTexture.render(25, 725);

					//update screen
					SDL_RenderPresent(gameRenderer);
				}
		}
	}
	end();

	return 0;
	}