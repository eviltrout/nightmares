#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "const.h"
#include "vga.h"
#include "timer.h"
#include "input.h"
#include <dos.h>

#define MAX_SPEED 4.0

#define COL_OFFSET 32

uint8 colors[5][3] = {
	{ 0, 0, 0 },
	{ 0, 0, 16 },
	{ 0, 0, 32 },
	{ 15, 15, 63 },
	{ 0, 0, 63 },
};

uint8 sprite[14][14] = {
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1 },
	{ 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 1 },
	{ 1, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2, 3, 1 },
	{ 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 1 },
	{ 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 1 },
	{ 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 1 },
	{ 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 1 },
	{ 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 1 },
	{ 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 1 },
	{ 1, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2, 3, 1 },
	{ 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 1 },
	{ 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};

typedef struct game_data_t {
  bool running;
	float speed;
	float y;
} game_data;

static uint16 counter = 0;

void setup_palette(void) {
  uint8 c;
  set_pal(16, 0, 0, 30);
  clear_screen(16);
}

#define MAX_Z 20.0

void draw_hallway(void) {
  uint16 x;
	float u = 0.0f;
	float v = 0.0f;

	float half_width = VIEWPORT_WIDTH * 0.5f;

  for (x=0; x<half_width; x++) {
    uint16 y0;
    uint16 y1;
    uint16 y;
    uint8 colx;
		uint8 coly;
		float z;
		float d;
		float h;

		d = ((float)x) / half_width;
		z = d * MAX_Z;
    y0 = (uint8)floor((z / MAX_Z) * (VIEWPORT_HEIGHT * 0.5));
    y1 = VIEWPORT_HEIGHT - y0;
		h = 14.0 / (y1 - y0);

		u += ((d * 1.0) + 0.1);
		if (u > 13.0) {
			u -= 13.0;
		}
		
		colx = (uint8) floor(u);
		
		v = 0.0;
    for (y=y0; y<y1; y++) {
			uint8 col;
			v += h;
			if (v > 13.0) {
				v = 13.0;
			}
			coly = (uint8) floor(v);
			col = COL_OFFSET + (coly * 14) + colx;
      plot(x, y, col);
      plot(VIEWPORT_WIDTH-x-1, y, col);
    }
  }
}

void update(float delta, game_data *data) {
	uint8 key;

	data->y += (data->speed * delta);

  key = get_key();
  switch(key) {
    case 1: // ESC
      data->running = false;
      break;
    case 17: // W
    case 72: // UP
			data->speed += delta;
			if (data->speed > MAX_SPEED) {
				data->speed = MAX_SPEED;
			}
      break;
  }
}

void render(game_data *data) {
  uint8 offset = (uint8) floor(data->y * 0.2);

	uint8 x, y;
	uint8 i = 0;
	for (y=0; y<14; y++) {
		for (x=0; x<14; x++) {
			uint8 col = sprite[y][(x + offset) % 14];
			set_pal(COL_OFFSET + i, colors[col][0], colors[col][1], colors[col][2]);
			i++;
		}
	}
}

void main_loop(game_data *data) {

  unsigned long start = readtimer();
	float delta = 0.0;

  while (data->running) {
    unsigned long stop = readtimer();
    unsigned long elapsed = elapsedtime(start, stop);
    start = stop;
		delta = ((float) elapsed) / 18.0f;

    update(delta, data);
    wait_vsync();
    render(data);
  }
}

int main()
{
  game_data data_stack;
  game_data *data = &data_stack;
  data->running = true;
  data->y = 0.0f;
	data->speed = 2.0f;

	initializetimer();

  video_mode(0x13);
  setup_palette();
  draw_hallway();
  main_loop(data);

  video_mode(0x2);
	restoretimer();

  return 0;
}

