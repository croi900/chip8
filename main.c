#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


#define RAM_SIZE 4096
#define RAM_START 0x0
#define RAM_PROG_START 0x200
#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32

#define STACK_SIZE 16
#define REGISTER_NUM 16


typedef uint8_t byte;
typedef uint16_t word;

//RAM of chip, as per the technical
// specifications, it is 4096 bytes
byte ram[RAM_SIZE]; 

//16 8-bit registers, Vx where x is 
//from 0 to F, in our case, 
//we access a register as V[x];
byte V[REGISTER_NUM];

//The callstack holds 16 16-bit
//addresses to the memory location
//where you should return when a
//subroutine is finsihed.

//sp is the stack pointer, it is
//decremented and incremented as
//we push or pop addresses to the
//stack
word callstack[STACK_SIZE];
byte sp = 0;

