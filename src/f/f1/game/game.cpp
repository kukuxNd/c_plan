
#include "game.h"
#include <cstdio>

game * game::self = nullptr;

game::game(){

}

game::~game(){

}

void game::init(){

}

void game::start(){

}

void game::loop(){
     //
     while(1){

        printf("game loop \n");
        usleep(10);
     }
}


void game::end(){

}