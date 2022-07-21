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
					if(state == draw && event.button.button == SDL_BUTTON_LEFT){
						//Turns a bit on if off and vice versa
						render_bitmap[event.button.y/CELL_SIZE] ^= 1 << (31 - (event.button.x/CELL_SIZE));

						/*	
						//Debug bit mouse placement
						for(int i = 0; i < NUM_CELL_Y; i++){
							for(int j = 0; j < NUM_CELL_X; j++){
								printf("%d", (render_bitmap[i] >> (31 - j)) & 1);
							}
							printf("\n");
					
						}
						printf("\n");
						*/
					}
					if(event.button.button == SDL_BUTTON_RIGHT){
						if(state == eval){
							state = draw;
						}
						else{
							state = eval;
						}
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


		if(state == eval){
			for(int i = 0; i < 32; i++){
				for(int j = 0; j < 32; j++){
					int cell_state = render_bitmap[i] >> (31 - j) & 1;
					int adjacent_lcells = 0; 
					if(j > 0)
						adjacent_lcells += render_bitmap[i] >> (32 - j) & 1;
					if(j < 31)
						adjacent_lcells += render_bitmap[i] >> (30 - j) & 1;
					if(i > 0)
						adjacent_lcells += render_bitmap[i-1] >> (31 - j) & 1;
					if(i < 31)
						adjacent_lcells += render_bitmap[i+1] >> (31 - j) & 1;
					if(i > 0 && j > 0)
						adjacent_lcells += render_bitmap[i-1] >> (32 - j) & 1;
					if(i < 31 && j < 31)
						adjacent_lcells += render_bitmap[i+1] >> (30 - j) & 1;
					if(i > 0 && j < 31)
						adjacent_lcells += render_bitmap[i-1] >> (30 - j) & 1;
					if(i < 31 && j > 0)
						adjacent_lcells += render_bitmap[i+1] >> (32 - j) & 1;
					//printf("%d", adjacent_lcells); for debugging
				}
				//printf("\n"); For debugging
			}
			state = draw;
		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

		for(int i = 0; i < NUM_CELL_Y; i++){
			for(int j = 0; j < NUM_CELL_X; j++){
				if(render_bitmap[i] >> (31 - j) & 1){
					SDL_Rect rect = {j*CELL_SIZE, i*CELL_SIZE, CELL_SIZE, CELL_SIZE};
					
					SDL_RenderFillRect(renderer, &rect);
				}
			}
		}
					
		SDL_RenderPresent(renderer);
	}

	SDL_Quit();
	return 0;
}
