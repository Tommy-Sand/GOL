#include "main.h"


int main(int argc, char **argv){
	if(SDL_Init(SDL_INIT_VIDEO) != 0){
		SDL_Log("Unable to initalize SDL: %s", SDL_GetError());
		return 1;
	}

	SDL_Window *window;

	window = SDL_CreateWindow(
		WINDOW_TITLE,
		WINDOW_X,
		WINDOW_Y,
		WINDOW_W,
		WINDOW_H,
		WINDOW_FLAG);

	if(window == NULL){
		SDL_Log("Unable to initalize window: %s", SDL_GetError());
		return 1;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Event event;
	while(1){
		while(SDL_PollEvent(&event) == 1){
			if((unsigned int) event.type == (unsigned int) SDL_Quit){
				return 1;
			}
		}
	}

	SDL_Quit();
	return 0;
}
