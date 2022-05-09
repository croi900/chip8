#include <stdlib.h>
#include <stdio.h>

#define RAM_SIZE 4096
#define RAM_START 0x0
#define RAM_PROG_START 0x200
#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32

typedef unsigned char byte;

byte ram[RAM_SIZE];

