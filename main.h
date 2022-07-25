#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define WINDOW_TITLE "John Conway's Game of Life"
#define WINDOW_X 300
#define WINDOW_Y 300
#define WINDOW_W 1024
#define WINDOW_H 1024
#define WINDOW_FLAG 0

#define NUM_CELL_X 32
#define NUM_CELL_Y 32
#define NUM_CELL_X_0 31
#define NUM_CELL_Y_0 31
#define CELL_SIZE 32

enum {draw = 0, eval = 1} state = draw;

unsigned int *render_bitmap;
bool run = 1;

void handle_event(unsigned int *render_bitmap);
int render(SDL_Renderer *renderer, unsigned int *render_bitmap);
unsigned int *evaluate_cells(unsigned int *render_bitmap);
