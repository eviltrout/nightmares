#ifndef _VGA_H
#define _VGA_H

void video_mode(uint16 mode);
void plot(uint16 x, uint16 y, uint8 col);
void clear_screen(uint8 col);
void set_pal(uint8 idx, uint8 r, uint8 g, uint8 b);
void vert_line(uint16 x, uint16 y0, uint16 y1, uint8 col);
void full_line(uint16 y, uint8 col);
void wait_vsync(void);

#endif
