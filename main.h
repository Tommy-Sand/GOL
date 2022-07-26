#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define WINDOW_TITLE "John Conway's Game of Life"
#define WINDOW_X 300
#define WINDOW_Y 300
int WINDOW_W;
int WINDOW_H;
#define WINDOW_FLAG 0

int NUM_CELL_X = 64;
int NUM_CELL_Y = 64;
int NUM_CELL_X_0;
int NUM_CELL_Y_0;
int CELL_SIZE;

enum {draw = 0, eval = 1} state = draw;

uint32_t **render_bitmap;
bool run = 1;
void window_dim();
void handle_event(uint32_t**render_bitmap);
int render(SDL_Renderer *renderer, uint32_t **render_bitmap);
uint32_t **evaluate_cells(uint32_t **render_bitmap);
