#pragma once
#include "config.h"
#include "xorshift.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef void(*instruction_t)(byte*);


//A vector of pointers to functions, each
//entry in the vector represents an instruction
//of the chip-8 architecture, this is useful for
//easily interpreting instructions

//Clears the screen
void ins_00E0(byte* ins){
    display[0] = 0;
}

// The interpreter sets the program counter 
//to the address at the top of the stack, 
//then subtracts 1 from the stack pointer.
void ins_00EE(byte* ins){
    pc = callstack[sp];
    sp--;
}

//Helper function to get the last three nibbles
//from two bytes. Used for Xnnn-type instructions
//where nnn are 3 nibbles that represent and address.
word get_addy(byte* ins){
    word addy = 0; // 00000000 00000000
    
    byte cb = *ins; //cb = [first byte]
    cb &= (byte)(-1)>>(sizeof(byte)*4); //get lower half of cb
    
    addy |= cb; // 00000000 0000[lower half]
    
    cb = *(ins+1); //cb = [2nd byte]
    
    addy<<=sizeof(byte)*8; // 0000[lower half] 00000000
    addy|= cb; //0000[lower half] [2nd byte]

    return addy;
}

// Jump to location nnn.

// The interpreter sets the program counter to nnn.

void ins_1nnn(byte* ins){
    pc = get_addy(ins);
}

//Call subroutine at nnn.

//The interpreter increments the stack pointer,
//then puts the current PC on the top of the 
//stack. The PC is then set to nnn.
void ins_2nnn(byte* ins){
    sp++;
    word addy = get_addy(ins);
    callstack[sp] = addy;
    pc = addy;
}

//Skip next instruction if Vx = kk.

//The interpreter compares register Vx to kk,
//and if they are equal, increments the 
//program counter by 2.
void ins_3xkk(byte* ins){
    byte x = *ins & ((byte)(-1)>>sizeof(byte)*4);
    byte cb = *(ins+1);
        
    if(cb == V[x])
        pc++;

    //We only increment once as there is a
    //default incrementation by the interpreter
    //thus, we skip to the next instruction 
    //immediately.

}

//Skip next instruction if Vx != kk.

//The interpreter compares register Vx to kk,
//and if they are not equal, increments the 
//program counter by 2.
void ins_4xkk(byte* ins){
    byte x = *ins & ((byte)(-1)>>sizeof(byte)*4);  // get lower nibble
    byte cb = *(ins+1);
        
    if(cb != V[x])
        pc++;
}

//Skip next instruction if Vx = Vy.

//The interpreter compares register Vx 
//to register Vy, and if they are equal,
//increments the program counter by 2.
void ins_5xy0(byte* ins){
    byte x = *ins & ((byte)(-1)>>sizeof(byte)*4); // get lower nibble
    byte y = *(ins+1)>>sizeof(byte)*4; //get higher nibble
    // printf("%x %x",x,y);
    if(V[x] == V[y])
        pc++;
}

//Set Vx == kk

//The interpreter puts the value kk
//in register Vx.
void ins_6xkk(byte* ins){
    byte x = *ins & ((byte)(-1)>>sizeof(byte)*4);
    V[x] = *(ins + 1);
}

//Set Vx = Vx + kk

//Adds value kk to Vx and sotres 
//it in Vx.
void ins_7xkk(byte* ins){
    byte x = *ins & ((byte)(-1)>>sizeof(byte)*4);
    V[x] += *(ins + 1);
}

//Assigns the value in Vy to Vx
void ins_8xy0(byte* ins){
    byte x = *ins & ((byte)(-1)>>sizeof(byte)*4); // get lower nibble
    byte y = *(ins+1)>>sizeof(byte)*4; //get higher nibble

    V[x] = V[y];
}

//Performs a bitwise OR on the values
//in Vx and Vy and stores the resul in Vx
void ins_8xy1(byte* ins){
    byte x = *ins & ((byte)(-1)>>sizeof(byte)*4); // get lower nibble
    byte y = *(ins+1)>>sizeof(byte)*4; //get higher nibble

    V[x] = V[x] | V[y];
}

//Performs a bitwise AND on the values
//in Vx and Vy and stores the resul in Vx
void ins_8xy2(byte* ins){
    byte x = *ins & ((byte)(-1)>>sizeof(byte)*4); // get lower nibble
    byte y = *(ins+1)>>sizeof(byte)*4; //get higher nibble

    V[x] = V[x] & V[y];
}

//Performs a bitwise XOR on the values
//in Vx and Vy and stores the resul in Vx
void ins_8xy3(byte* ins){
    byte x = *ins & ((byte)(-1)>>sizeof(byte)*4); // get lower nibble
    byte y = *(ins+1)>>sizeof(byte)*4; //get higher nibble

    V[x] = V[x] ^ V[y];
}

//Performs the operation ADD on the values
//in Vx and Vy and if the result is greater
//than 255, VF is set to 1, otherwise 0.
void ins_8xy4(byte* ins){
    byte x = *ins & ((byte)(-1)>>sizeof(byte)*4); // get lower nibble
    byte y = *(ins+1)>>sizeof(byte)*4; //get higher nibble

    word res = V[x] + V[y];
    if(res > 255)
        V[0xF] = 1;
    else
        V[0xF] = 0;
    V[x] = V[x] + V[y];
}

//Performs the operation SUB on the values
//of Vx adn Vy, if Vx > Vy, VF is set to 1
//otherwise it is 0, then Vy is subtracted 
//from Vx and the result is stored in Vx
void ins_8xy5(byte* ins){
    byte x = *ins & ((byte)(-1)>>sizeof(byte)*4); // get lower nibble
    byte y = *(ins+1)>>sizeof(byte)*4; //get higher nibble

    if(V[x] > V[y])
        V[0xF] = 1;
    else
        V[0xF] = 0;
    
    V[x] = V[x] - V[y];
}

//Vx = Vx SHR 1

//if the LSB of Vx is 1 then VF is 
//set to one then Vx is divided by 
//two, for some reason, Vy is 
//not used in this instruction.
void ins_8xy6(byte* ins){
    byte x = *ins & ((byte)(-1)>>sizeof(byte)*4); // get lower nibble
    V[0xF] = V[x] & 1;
    V[x] = V[x] / 2;
}

//Performs the operation SUBN on the values
//of Vx adn Vy, if Vy > Vx, VF is set to 1
//otherwise it is 0, then Vy is subtracted 
//from Vx and the result is stored in Vx
void ins_8xy7(byte* ins){
    byte x = *ins & ((byte)(-1)>>sizeof(byte)*4); // get lower nibble
    byte y = *(ins+1)>>sizeof(byte)*4; //get higher nibble

    if(V[y] > V[x])
        V[0xF] = 1;
    else
        V[0xF] = 0;
    
    V[x] = V[y] - V[x];
}

//Vx = Vx SHL 1

//if the MSB of Vx is 1 then VF is 
//set to one then Vx is multiplied by 
//two, for some reason, Vy is 
//not used in this instruction.
void ins_8xyE(byte* ins){
    byte x = *ins & ((byte)(-1)>>sizeof(byte)*4); // get lower nibble

    V[0xF] = (V[x]>>(sizeof(byte)-1)) & 1;
    V[x] = V[x] * 2;
}

// Skip next instruction if Vx != Vy.

// The values of Vx and Vy are compared,
// and if they are not equal, 
// the program counter is increased 
// by 2.
void ins_9xy0(byte* ins){
    byte x = *ins & ((byte)(-1)>>sizeof(byte)*4); // get lower nibble
    byte y = *(ins+1)>>sizeof(byte)*4; //get higher nibble

    if(V[x] != V[y])
        pc++;
}

//The value of register I is set to nnn.
void ins_Annn(byte* ins){
    byte n = *ins & ((byte)(-1)>>sizeof(byte)*4); // get lower nibble from first byte
    byte nn = *(ins+1); //get second byte (last two thirds)

    I = nn;
    I = I + n<<(sizeof(word)/2);
}

//The program counter is set to nnn 
//plus the value of V0.
void ins_Bnnn(byte* ins){
    byte n = *ins & ((byte)(-1)>>sizeof(byte)*4); // get lower nibble from first byte
    byte nn = *(ins+1); //get second byte (last two thirds)

    word addy = nn + n<<(sizeof(word)/2);
    
    pc = addy + V[0];
}

//Generates a random byte.
// byte rbyte(byte seed){
    
// }

//The interpreter generates a 
//random number from 0 to 255, 
//which is then ANDed with the 
//value kk. The results are 
//stored in Vx.
void ins_Cxkk(byte* ins){
    byte rnd = xor_byte();
    rnd &= *(ins+1); //get second byte 
    byte x = *ins & ((byte)(-1)>>sizeof(byte)*4); // get lower nibble

    V[x] = rnd;
}


// Display n-byte sprite starting
// at memory location I at 
//(Vx, Vy), set VF = collision.

//The interpreter reads n bytes 
// from memory, starting at the 
// address stored in I. These bytes 
// are then displayed as sprites on 
// screen at coordinates (Vx, Vy). 
// Sprites are XORed onto the existing 
// screen. If this causes any pixels 
// to be erased, VF is set to 1, 
// otherwise it is set to 0. If the 
// sprite is positioned so part of 
// it is outside the coordinates of 
// the display, it wraps around to 
// the opposite side of the screen. 
void ins_Dxyn(byte* ins){
    byte x = *ins & ((byte)(-1)>>sizeof(byte)*4); // get lower nibble
    byte y = *(ins+1)>>sizeof(byte)*4; //get higher nibble
    byte n = *(ins+1)&((byte)(-1)>>sizeof(byte)*4); // get n

    x = V[x];
    y = V[y];

    for(byte i = 0; i < n; ++i){
        byte b = ram[I + i];
        for(byte j = 0; j < 8; ++j){
            byte px = (x + j) % DISPLAY_WIDTH;
            byte py = (y + i) % DISPLAY_HEIGHT;
            byte pval = (b>>j)&1;

            printf("%d %d\n",px,py);
            if(display[py * DISPLAY_WIDTH + px] == 1 && pval)
                V[0xF] = 1;
            display[py * DISPLAY_WIDTH + px] ^= pval;
        }
    }
}

instruction_t ins[] = {
    ins_00E0,
    ins_00EE,
    ins_1nnn,
    ins_2nnn,
    ins_3xkk,
    ins_4xkk
};
