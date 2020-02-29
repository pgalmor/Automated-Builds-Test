#include <iostream>
#include "SDL/include/SDL.h"
#include "SDL_image/include/SDL_image.h"
#include "SDL_mixer/include/SDL_mixer.h"

#pragma comment(lib, "SDL/libx86/SDL2.lib")
#pragma comment(lib, "SDL/libx86/SDL2main.lib")
#pragma comment(lib, "SDL_image/libx86/SDL2_image.lib")
#pragma comment(lib, "SDL_mixer/libx86/SDL2_mixer.lib")

using namespace std;

int main(int argc, char **argv) {
	bool winClose = true;
	int SDL_Init(SDL_INIT_VIDEO);
	int SDL_Init2(SDL_INIT_AUDIO);
	int IMG_Init(IMG_INIT_PNG);
	int Mix_Init(MIX_INIT_OGG);
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);

	int SCREEN_WEIGHT = 1920, SCREEN_HEIGHT = 1080;

	SDL_Window *mainFin;
	SDL_Renderer *winRender;
	SDL_Rect rectP;
	SDL_Rect rectP2;
	SDL_Rect rectB;
	SDL_Rect rectB2;
	SDL_Rect rectBack;
	SDL_Surface *Load_Surf;
	SDL_Texture *Player;
	SDL_Texture *Background;
	SDL_Texture *Bullet;

	bool shoot = false;
	mainFin = SDL_CreateWindow("MyAwesomeGame by Pol Galan", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WEIGHT, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
	winRender = SDL_CreateRenderer(mainFin, -1, SDL_RENDERER_ACCELERATED);
	SDL_Event event;
	
	Load_Surf = IMG_Load("Assets/Pingu.png");
	if (!Load_Surf) {
		winClose = false;
	}
	Player = SDL_CreateTextureFromSurface(winRender, Load_Surf);
	SDL_FreeSurface(Load_Surf);

	Load_Surf = IMG_Load("Assets/Bullet.png");
	if (!Load_Surf) {
		winClose = false;
	}
	Bullet = SDL_CreateTextureFromSurface(winRender, Load_Surf);
	SDL_FreeSurface(Load_Surf);

	Load_Surf = IMG_Load("Assets/Igloo.jpg");
	if (!Load_Surf) {
		winClose = false;
	}
	Background = SDL_CreateTextureFromSurface(winRender, Load_Surf);
	SDL_FreeSurface(Load_Surf);

	
	rectP.x = 0;
	rectP.y = 0;
	rectP.w = 480;
	rectP.h = 480;

	rectP2.x = 20;
	rectP2.y = 10;
	rectP2.w = 240;
	rectP2.h = 240;

	rectB.x = 0;
	rectB.y = 0;
	rectB.w = 170;
	rectB.h = 170;

	rectB2.x = 0;
	rectB2.y = 0;
	rectB2.w = 60;
	rectB2.h = 60;

	rectBack.x = 0;
	rectBack.y = 0;
	rectBack.w = 1920;
	rectBack.h = 1080;

	while (winClose) {
		if (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				winClose = false;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_LEFT:
					if (rectP2.x != 0) {
						rectP2.x = rectP2.x - 15;
					}
					break;
				case SDLK_RIGHT:
					if (rectP2.x != 400) {
						rectP2.x = rectP2.x + 15;
					}
					break;
				case SDLK_UP:
					rectP2.y = rectP2.y - 15;
					break;
				case SDLK_DOWN:
					rectP2.y = rectP2.y + 15;
					break;
				case SDLK_SPACE:
					shoot = true;
					rectB2.x = rectP2.x + 240;
					rectB2.y = rectP2.y + 89;
					break;
				case SDLK_ESCAPE:
					winClose = false;
					break;
				}
			}
		}
		SDL_RenderCopy(winRender, Background, NULL, NULL);
		SDL_RenderCopy(winRender, Player, &rectP, &rectP2);
		if (shoot == true) {
			SDL_RenderCopy(winRender, Bullet, &rectB, &rectB2);
			rectB2.x += 4;
			if (rectB2.x >= SCREEN_WEIGHT) {
				rectB2.x = rectP2.x;
				shoot = false;
			}
		}
		SDL_RenderPresent(winRender);
	}
	SDL_DestroyTexture(Background);
	SDL_DestroyTexture(Player);
	SDL_DestroyTexture(Bullet);
	SDL_DestroyRenderer(winRender);
	SDL_DestroyWindow(mainFin);
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();

	return 0;
}