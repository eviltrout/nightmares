#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "const.h"
#include "panels.h"
#include "vga.h"
#include "timer.h"
#include "input.h"
#include <dos.h>

#define MAX_HEIGHT 160
#define COLORS 8
#define SHADES 25

static uint8 pal[COLORS][3] = {
	{ 0, 0, 0 },
	{ 56, 56, 63 },
	{ 44, 44, 48 },
	{ 0, 0, 63 },
	{ 0, 0, 22 },
	{ 0, 0, 0 },
	{ 0, 0, 0 },
	{ 0, 0, 0 },
};

typedef struct cmd_t {
  uint16 start;
  uint8 count;
  uint8 shade;
  uint8 texels[MAX_HEIGHT];
} cmd;

typedef struct game_data_t {
  bool running;
	float speed;
	float y;

  cmd cmds[VIEWPORT_WIDTH/2];
} game_data;

void setup_palette(void) {
  uint8 i, j;
  float col;

  for (j=0; j<SHADES; j++) {
    float intensity = ((float)SHADES - (float)j) / (float)SHADES;
    for (i=0; i<COLORS; i++) {
      uint8 r = (uint8) floor((float) pal[i][0] * intensity);
      uint8 g = (uint8) floor((float) pal[i][1] * intensity);
      uint8 b = (uint8) floor((float) pal[i][2] * intensity);
      set_pal(j * COLORS + i, r, g, b);
    }
  }

  for (j=0; j<32; j++) {
    set_pal(200+j, 0, 0, j * 2);
  }
  clear_screen(216);

  for (j=0; j<199; j++) {
    if (j < 80) {
      col = (float) j / 80.0;
    } else {
      col = 1.0 - ((float) j - 80.0) / 200.0;
    }
    full_line(j, 200+32-(col * 32.0));
  }

}

#define MAX_Z 20.0

void calculate_hallway(game_data *data) {
  float x;
	float half_width = VIEWPORT_WIDTH * 0.5f;

  float cols_per_shade = half_width / (float) SHADES;

  cmd *c = data->cmds; 
  uint16 i = 0;
  for (x=0; x<half_width; x++) {
    uint16 y0;
    uint16 y1;
    uint16 y;
    uint8 colx;
		uint8 coly;
		float z;
		float d;
		float h;
    float sy;

		d = x / half_width;
		z = d * MAX_Z;
    y0 = (uint8)floor((z / MAX_Z) * (VIEWPORT_HEIGHT * 0.5));
    y1 = VIEWPORT_HEIGHT - y0;

    c->start = (y0 * 320) + x;
    c->count = y1 - y0;
    c->shade = (uint8) floor(x / cols_per_shade) * 8;

    sy = (float) 64.0 / (float)c->count;
    for (i=0; i<c->count; i++) {
      c->texels[i] = (uint8) floor(sy * (float)i + 0.5);
      if (c->texels[i] > 63) {
        c->texels[i] = 63;
      }
    }
    c++;
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
  }
}

void render(game_data *data) {
  uint16 x;
  uint8 col = ((int) data->y) % 32;

  cmd *cmds = data->cmds;

  uint8 *tex = panel;
  uint16 off = col * 64;
  uint8 shade = 0;

  asm {
    MOV AX, 0A000h
    MOV ES, AX
    MOV CX, 0
    MOV SI, cmds
  }
  render_columns:
  asm {
    MOV DI, [SI]  
    MOV BH, 160
    MOV BL, [SI+3]
    MOV shade, BL
    MOV BL, [SI+2]
    SUB BH, BL
    ADD SI, 4
    MOV AX, 0
  }
  render_slice:
  asm {
    PUSH DI
    MOV DI, tex
    XOR AH, AH
    MOV AL, [SI]
    ADD DI, AX
    MOV AX, off
    ADD DI, AX
    MOV AL, [DI]
    ADD AL, shade
    POP DI

    MOV ES:[DI], AL
    ADD DI, 319
    SUB DI, CX
    SUB DI, CX
    MOV ES:[DI], AL
    ADD DI, CX
    ADD DI, CX
    INC DI
    INC SI

    DEC BL
    JNZ render_slice

    MOV AX, off
    ADD AX, 64
    CMP AX, 2048
    JL finish_loop
    MOV AX, 0
  }
  finish_loop:
  asm {
    MOV off, AX
    MOV DL, BH
    XOR DH, DH
    ADD SI, DX
    INC CX
    CMP CX, 159
    JLE render_columns
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
	data->speed = 1.0f;

	initializetimer();

  video_mode(0x13);
  setup_palette();
  calculate_hallway(data);
  main_loop(data);

  video_mode(0x2);
	restoretimer();

  return 0;
}

