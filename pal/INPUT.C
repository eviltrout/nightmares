#include "const.h"

uint8 get_key(void) {
  uint8 key = 0;
  asm {
    MOV AH, 01h
    INT 16h
    JZ no_char
    MOV AH, 0
    INT 16h
    MOV key, AH
  }
  return key;

  no_char:
  return 0;
}


