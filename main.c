#include "instructions.h"
#include "xorshift.h"
#include <stdio.h>
#include "raylib.h"
#define USE_RAYLIB
void show_display(){
    // system("clear");
    #ifdef USE_CONSOLE
    for(int i = 0; i < DISPLAY_HEIGHT; ++i){
        for(int j = 0; j < DISPLAY_WIDTH; ++j){
            putc('0' + display[i*DISPLAY_WIDTH+j],stdout);
        }
        putc('\n',stdout);
    }
    #endif

    #ifdef USE_RAYLIB
        for(int i = 0; i < DISPLAY_HEIGHT; ++i){
            for(int j = 0; j < DISPLAY_WIDTH; ++j){
                if(display[i*DISPLAY_WIDTH+j]){
                    DrawRectangle(j*8,i*8,8,8,WHITE);
                }else{
                    DrawRectangle(j*8,i*8,8,8,BLACK);
                }
            }
        }
    #endif
}

int main(){
    const int screenWidth = DISPLAY_WIDTH * 8;
    const int screenHeight = DISPLAY_HEIGHT * 8;
    int pc = 0x200;
    InitWindow(screenWidth, screenHeight, "chip8croi");

    SetTargetFPS(60);              
    while (!WindowShouldClose())    
    {
        
        BeginDrawing();

            ClearBackground(BLACK);


        EndDrawing();
        pc++;
    }

   
    CloseWindow();        

    return 0;


    
    
    // for(int i = 0; i < 7; ++i){
    //     V[0] = i*6;
    //     V[1] = 0;
    //     I = 5*i;
    //     ins_Dxyn(instr);
    //     printf("\n");
    // }
    
    // show_display();

    // return 0;
    
}
