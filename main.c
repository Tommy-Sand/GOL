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

		

		if(state == eval){
			evaluate_cells(render_bitmap);
			state = draw;
		}

		if(render(renderer, render_bitmap)){
			return 1;
		}
		SDL_RenderPresent(renderer);
	}

	SDL_Quit();
	return 0;
}

int render(SDL_Renderer *renderer, unsigned int *render_bitmap){
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	if(SDL_RenderClear(renderer) != 0){
		SDL_Log("Unable to clear screen: %s", SDL_GetError());
		return 1;
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
	return 0;

}

void evaluate_cells(unsigned int *render_bitmap){
	unsigned int render_bitmap_event[NUM_CELL_Y][NUM_CELL_X] = {0};
	for(int i = 0; i < NUM_CELL_Y; i++){
		for(int j = 0; j < NUM_CELL_X; j++){
			int cell_state = render_bitmap[i] >> (NUM_CELL_X_0 - j) & 1;
			int adjacent_lcells = 0; 
			if(j > 0)
				render_bitmap_event[i][j] += render_bitmap[i] >> (NUM_CELL_X_0 + 1 - j) & 1;
			if(j < NUM_CELL_X_0 )
				render_bitmap_event[i][j] += render_bitmap[i] >> (NUM_CELL_X_0 - 1 - j) & 1;
			if(i > 0)
				render_bitmap_event[i][j]+= render_bitmap[i-1] >> (NUM_CELL_X_0  - j) & 1;
			if(i < NUM_CELL_Y_0 )
				render_bitmap_event[i][j]+= render_bitmap[i+1] >> (NUM_CELL_X_0  - j) & 1;
			if(i > 0 && j > 0)
				render_bitmap_event[i][j] += render_bitmap[i-1] >> (NUM_CELL_X_0 + 1 - j) & 1;
			if(i < NUM_CELL_X_0  && j < NUM_CELL_Y_0)
				render_bitmap_event[i][j] += render_bitmap[i+1] >> (NUM_CELL_X_0 - 1 - j) & 1;
			if(i > 0 && j < NUM_CELL_X_0 )
				render_bitmap_event[i][j] += render_bitmap[i-1] >> (NUM_CELL_X_0 - 1 - j) & 1;
			if(i < NUM_CELL_Y_0  && j > 0)
				render_bitmap_event[i][j] += render_bitmap[i+1] >> (NUM_CELL_X_0 + 1 - j) & 1;
			printf("%d", render_bitmap_event[i][j]);// for debugging
					
		}
	printf("\n");// For debugging
	}


}
