@Echo off

bcc -1 -v -etexture.exe vga.c input.c texture.c timer.c timer1.asm
IF ERRORLEVEL 1 GOTO End

td texture.exe

:End

