#include <dos.h>
#include "const.h"

uint8 far *VGA = MK_FP(0xa000, 0);

void wait_vsync() {
  asm MOV DX, 03DAh
  in_sync:
  asm {
    IN AL, DX
    TEST AL, 8
    JNZ in_sync
  }
  wait:
  asm {
    IN AL, DX
    TEST AL, 8
    JZ wait
  }
}

void video_mode(uint16 mode) {
  asm {
    MOV AX, mode
    INT 10h
  }
}

void plot(uint16 x, uint16 y, uint8 col) {
  VGA[y * 320 + x] = col;
}

void vert_line(uint16 x, uint16 y0, uint16 y1, uint8 col) {
  asm {
    PUSH 0A000h
    POP ES

    MOV AX, y0
    MOV CX, 320
    MUL CX
    ADD AX, x

    MOV DI, AX
    MOV AL, col

    MOV CX, y1
    SUB CX, y0
  }
  draw_column:
  asm {
    MOV ES:[DI], AL
    ADD DI, 320
    DEC CX
    JNZ draw_column
  }
}


void full_line(uint8 y, uint8 col) {
  asm {
    PUSH 0A000h
    POP ES
    MOV AL, y
    XOR AH, AH
    MOV CX, 320
    MUL CX
    MOV DI, AX

    MOV CX, 320
    MOV AL, col
    MOV AH, col
    MOV CX, 160
    REP STOSW
  }
}

void clear_screen(uint8 col) {
  asm {
    push 0A000h
    pop ES
    mov DI, 0

    mov AL, col
    mov AH, col
    mov CX, 0FFFFH
    REP STOSB
  }
}

void set_pal(uint8 idx, uint8 r, uint8 g, uint8 b) {
  asm {
    MOV DX, 3C6h
    MOV AL, 0FFh
    OUT DX, AL

    MOV AL, idx
    MOV DX, 3C8h
    OUT DX, AL

    MOV DX, 3C9h
    MOV AL, r
    OUT DX, AL
    MOV AL, g
    OUT DX, AL
    MOV AL, b
    OUT DX, AL
  }
}


