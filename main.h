#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define WINDOW_TITLE "John Conway's Game of Life"
#define WINDOW_X 300
#define WINDOW_Y 300
#define WINDOW_W 1600
#define WINDOW_H 1024
#define WINDOW_FLAG 0

#define NUM_CELL_X 64
#define NUM_CELL_Y 32
#define NUM_CELL_X_0 63
#define NUM_CELL_Y_0 31
#define CELL_SIZE 32

enum {draw = 0, eval = 1} state = draw;

uint32_t **render_bitmap;
bool run = 1;

void handle_event(uint32_t**render_bitmap);
int render(SDL_Renderer *renderer, uint32_t **render_bitmap);
uint32_t **evaluate_cells(uint32_t **render_bitmap);
