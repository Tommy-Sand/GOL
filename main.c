#include "main.h"


int main(int argc, char **argv){
	if(SDL_Init(SDL_INIT_VIDEO) != 0){
		SDL_Log("Unable to initalize SDL: %s", SDL_GetError());
		return 1;
	}

        SDL_Window *window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_X, WINDOW_Y, WINDOW_W, WINDOW_H, WINDOW_FLAG);

        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if(window == NULL){
		SDL_Log("Unable to initalize window: %s", SDL_GetError());
		return 1;
	}
		
	while(run){
		while(SDL_PollEvent(&event) == 1){
			switch(event.type){
				case SDL_QUIT:
					run = false;
					break;

				case SDL_MOUSEBUTTONUP:
					if(event.button.button == SDL_BUTTON_LEFT){
						//Turns a bit on if off and vice versa
						render_bitmap[event.button.y/32] ^= 1 << (31 - (event.button.x/32));

						/*	
						//Debug bit mouse placement
						for(int i = 0; i < 32; i++){
							for(int j = 0; j < 32; j++){
								printf("%d", (render_bitmap[i] >> (31 - j)) & 1);
							}
							printf("\n");
					
						}
						printf("\n");
						*/
					}
			}
			if(run == false){
				break;
			}
		}		

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		if(SDL_RenderClear(renderer) != 0){
			SDL_Log("Unable to clear screen: %s", SDL_GetError());
			return 1;
		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

		for(int i = 0; i < 31; i++){
			for(int j = 0; j < 31; j++){
				if(render_bitmap[i] >> (31 - j) & 1){
					
					rect.x = j*32;
					rect.y = i*32;
					
					SDL_RenderFillRect(renderer, &rect);
				}
			}
		}
					
		SDL_RenderPresent(renderer);
	}

	SDL_Quit();
	return 0;
}
