#include "main.h"


int main(int argc, char **argv){

	window_dim();

	NUM_CELL_X_0 = NUM_CELL_X - 1;
	NUM_CELL_Y_0 = NUM_CELL_Y - 1;
	CELL_SIZE = WINDOW_W/NUM_CELL_X;

		

	if(SDL_Init(SDL_INIT_VIDEO) != 0){
		SDL_Log("Unable to initalize SDL: %s", SDL_GetError());
		return 1;
	}

        SDL_Window *window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_X, WINDOW_Y, WINDOW_W, WINDOW_H, WINDOW_FLAG);

        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	render_bitmap = (uint32_t **) calloc(NUM_CELL_Y, sizeof(uint32_t *)) ;
	for(int i = 0; i < NUM_CELL_Y; i++){
		render_bitmap[i] = (uint32_t *) calloc((int) ((NUM_CELL_X_0/32) + 1), sizeof(uint32_t));
	}

	//printf("num of uint32 columns used: %d\n", (int) ((NUM_CELL_X_0/32) + 1)); //only for debugging

	if(window == NULL){
		SDL_Log("Unable to initalize window: %s", SDL_GetError());
		return 1;
	}

	while(run){
		handle_event(render_bitmap);

		if(state == eval){
			uint32_t **temp = render_bitmap;
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

void window_dim(){
	printf("Input the height/width of the window: ");
	scanf("%d", &WINDOW_W);
	printf("Input the number of cells in x and y direction: ");
	scanf("%d", &NUM_CELL_X);
	WINDOW_W += 1
	WINDOW_H = WINDOW_W;
	NUM_CELL_Y = NUM_CELL_X;
}

void handle_event(uint32_t **render_bitmap){
	SDL_Event event;
	while(SDL_PollEvent(&event) == 1){
		switch(event.type){
			case SDL_QUIT:
				run = false;
				break;

			case SDL_MOUSEBUTTONUP:
				if(state == draw && event.button.button == SDL_BUTTON_LEFT && event.button.x > 0 && event.button.x < WINDOW_W && event.button.y > 0 && event.button.y < WINDOW_H){
					//Turns a bit on if off and vice versa
					uint32_t cell_block = event.button.x/(32 * CELL_SIZE);
					render_bitmap[event.button.y/CELL_SIZE][cell_block] ^= 1 << (31 - ((event.button.x/CELL_SIZE)) % 32);//32 being the size of an unsigned interger
					
					/*
					//Debug bit mouse placement
					for(int i = 0; i < NUM_CELL_Y; i++){
						for(int j = 0; j < NUM_CELL_X; j++){
							printf("%d", (render_bitmap[i][j/32] >> (31 - j)) & 1);
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

int render(SDL_Renderer *renderer, uint32_t **render_bitmap){
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	if(SDL_RenderClear(renderer) != 0){
		SDL_Log("Unable to clear screen: %s", SDL_GetError());
		return 1;
	}

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

	for(int i = 0; i <= NUM_CELL_X; i++){
		SDL_RenderDrawLine(renderer, i*CELL_SIZE, 0, i*CELL_SIZE, WINDOW_H);
	}

	for(int j = 0; j <= NUM_CELL_Y; j++){
		SDL_RenderDrawLine(renderer, 0, j*CELL_SIZE, WINDOW_W, j*CELL_SIZE); 
	}

	for(int i = 0; i < NUM_CELL_Y; i++){
		for(int j = 0; j < NUM_CELL_X; j++){
			if(render_bitmap[i][j/32] >> (31 - j) & 1){
				SDL_Rect rect = {j*CELL_SIZE, i*CELL_SIZE, CELL_SIZE, CELL_SIZE};
				
				SDL_RenderFillRect(renderer, &rect);
			}
		}
	}
	return 0;

}

unsigned int **evaluate_cells(uint32_t **render_bitmap){
	
	uint32_t **render_bitmap_event = (uint32_t **) calloc(NUM_CELL_Y, sizeof(uint32_t *));
	for(int i = 0; i < NUM_CELL_Y; i++){
		render_bitmap_event[i] = (uint32_t *) calloc((int) (NUM_CELL_X_0/32) + 1, sizeof(uint32_t));
	}

	for(int i = 0; i < NUM_CELL_Y; i++){
		for(int j = 0; j < NUM_CELL_X; j++){
			uint32_t cell_block = j/32; //32 being the size of uint32_t
			int cell_state = render_bitmap[i][cell_block] >> (NUM_CELL_X_0 - j) & 1;
			int adjacent_lcells = 0; 
			if(j > 0){
				if((j - 1)/32 == cell_block)//32 being the size of an uint32_t
					adjacent_lcells += render_bitmap[i][cell_block] >> (NUM_CELL_X_0 + 1 - j) & 1;
				else
					adjacent_lcells += render_bitmap[i][cell_block - 1] & 1;
			}
			if(j < NUM_CELL_X_0 ){
				if((j + 1)/32 == cell_block)//32 being the size of an uint32_t
					adjacent_lcells += render_bitmap[i][cell_block] >> (NUM_CELL_X_0 - 1 - j) & 1;
				else
					adjacent_lcells += render_bitmap[i][cell_block + 1] >> 31 & 1;
			}
			if(i > 0){
				adjacent_lcells += render_bitmap[i-1][cell_block] >> (NUM_CELL_X_0  - j) & 1;
			}
			if(i < NUM_CELL_Y_0 ){
				adjacent_lcells  += render_bitmap[i+1][cell_block] >> (NUM_CELL_X_0  - j) & 1;
			}	
			if(i > 0 && j > 0){
				if((j-1)/32 == cell_block)
					adjacent_lcells  += render_bitmap[i-1][cell_block] >> (NUM_CELL_X_0 + 1 - j) & 1;
				else
					adjacent_lcells  += render_bitmap[i-1][cell_block - 1] & 1;
			}	
			if(i < NUM_CELL_Y_0  && j < NUM_CELL_X_0){
				if((j + 1)/32 == cell_block){
					adjacent_lcells += render_bitmap[i+1][cell_block] >> (NUM_CELL_X_0 - 1 - j) & 1;
				}
				else
					adjacent_lcells  += render_bitmap[i+1][cell_block + 1] >> 31 & 1;
				
			}
			if(i > 0 && j < NUM_CELL_X_0){
				if((j + 1)/32 == cell_block)
					adjacent_lcells  += render_bitmap[i-1][cell_block]>> (NUM_CELL_X_0 - 1 - j) & 1;
				else
					adjacent_lcells  += render_bitmap[i-1][cell_block + 1] >> 31 & 1;

			}	
			if(i < NUM_CELL_Y_0  && j > 0){
				if((j - 1)/32 == cell_block)
					adjacent_lcells  += render_bitmap[i+1][cell_block]>> (NUM_CELL_X_0 + 1 - j) & 1;
				else
					adjacent_lcells  += render_bitmap[i+1][cell_block - 1] & 1;
			}
			//printf("%d", adjacent_lcells);// for debugging
			
			if(cell_state == 1){ // The cell is live
				if(adjacent_lcells == 2 || adjacent_lcells == 3)
					render_bitmap_event[i][cell_block] |= 1 << (31 - j);
			}
			else if(cell_state == 0){
				if(adjacent_lcells == 3)
					render_bitmap_event[i][cell_block] |= 1 << (31 - j);
			}
			//printf("%d", (render_bitmap_event[i][cell_block] >> (31 - j)) & 1);	
		}
	//printf("\n");// For debugging
	}
	return render_bitmap_event;
}
