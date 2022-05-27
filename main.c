#include "instructions.h"
#include "xorshift.h"
#include <stdio.h>

void show_display(){
    // system("clear");
    for(int i = 0; i < DISPLAY_HEIGHT; ++i){
        for(int j = 0; j < DISPLAY_WIDTH; ++j){
            putc('0' + display[i*DISPLAY_WIDTH+j],stdout);
        }
        putc('\n',stdout);
    }
}

int main(){
    byte instr[2] = {0xD0,0x15};
    
    for(int i = 0; i < 16; ++i){
        V[0] = 8*i;
        V[1] = 0;
        I = 5*i;
        ins_Dxyn(instr);
        printf("\n");
    }
    
    show_display();

    return 0;
    
}
