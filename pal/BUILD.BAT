@Echo off

bcc -1 -epal.exe vga.c input.c pal.c timer.c timer1.asm
IF ERRORLEVEL 1 GOTO End

pal.exe

:End
