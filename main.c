#include "main.h"


int main(int argc, char **argv){
	if(SDL_Init(SDL_INIT_VIDEO) != 0){
		SDL_Log("Unable to initalize SDL: %s", SDL_GetError());
		return 1;
	}

        SDL_Window *window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_X, WINDOW_Y, WINDOW_W, WINDOW_H, WINDOW_FLAG);

        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	render_bitmap = (unsigned int *) calloc(NUM_CELL_Y, sizeof(unsigned int)) ;

	if(window == NULL){
		SDL_Log("Unable to initalize window: %s", SDL_GetError());
		return 1;
	}

	while(run){
		handle_event(render_bitmap);

		if(state == eval){
			unsigned int *temp = render_bitmap;
			render_bitmap = evaluate_cells(render_bitmap);
			free(temp);
			state = draw;
		}

		if(render(renderer, render_bitmap)){
			return 1;
		}
		SDL_RenderPresent(renderer);
	}
	free(render_bitmap);
	SDL_Quit();
	return 0;
}

void handle_event(unsigned int *render_bitmap){
	SDL_Event event;
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

unsigned int *evaluate_cells(unsigned int *render_bitmap){
	unsigned int *render_bitmap_event = (unsigned int *) calloc(NUM_CELL_Y, sizeof(unsigned int));
	for(int i = 0; i < NUM_CELL_Y; i++){
		for(int j = 0; j < NUM_CELL_X; j++){
			int cell_state = render_bitmap[i] >> (NUM_CELL_X_0 - j) & 1;
			int adjacent_lcells = 0; 
			if(j > 0)
				adjacent_lcells += render_bitmap[i] >> (NUM_CELL_X_0 + 1 - j) & 1;
			if(j < NUM_CELL_X_0 )
				adjacent_lcells += render_bitmap[i] >> (NUM_CELL_X_0 - 1 - j) & 1;
			if(i > 0)
				adjacent_lcells += render_bitmap[i-1] >> (NUM_CELL_X_0  - j) & 1;
			if(i < NUM_CELL_Y_0 )
				adjacent_lcells  += render_bitmap[i+1] >> (NUM_CELL_X_0  - j) & 1;
			if(i > 0 && j > 0)
				adjacent_lcells  += render_bitmap[i-1] >> (NUM_CELL_X_0 + 1 - j) & 1;
			if(i < NUM_CELL_X_0  && j < NUM_CELL_Y_0)
				adjacent_lcells  += render_bitmap[i+1] >> (NUM_CELL_X_0 - 1 - j) & 1;
			if(i > 0 && j < NUM_CELL_X_0 )
				adjacent_lcells  += render_bitmap[i-1] >> (NUM_CELL_X_0 - 1 - j) & 1;
			if(i < NUM_CELL_Y_0  && j > 0)
				adjacent_lcells  += render_bitmap[i+1] >> (NUM_CELL_X_0 + 1 - j) & 1;
			//printf("%d", adjacent_lcells);// for debugging
			
			if(cell_state == 1){ // The cell is live
				if(adjacent_lcells == 2 || adjacent_lcells == 3)
					render_bitmap_event[i] |= 1 << (31 - j);
			}
			else if(cell_state == 0){
				if(adjacent_lcells == 3)
					render_bitmap_event[i] |= 1 << (31 - j);
			}
			//printf("%d", (render_bitmap_event[i] >> (31 - j)) & 1);	
		}
	//printf("\n");// For debugging
	}
	return render_bitmap_event;
}
