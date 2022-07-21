#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define WINDOW_TITLE "John Conway's Game of Life"
#define WINDOW_X 300
#define WINDOW_Y 300
#define WINDOW_W 640
#define WINDOW_H 640
#define WINDOW_FLAG 0

enum {draw = 0, eval = 1} state = draw;

SDL_Event event;

unsigned int render_bitmap[32] = {0};

bool run = 1;
