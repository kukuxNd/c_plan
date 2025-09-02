#include "core.h"
#include <cstdio>
#include <functional>
#include "game/game.h"

core::core(fw* _fw):m_pfw(_fw){
    _fw->del_init += std::bind(&core::init, this, std::placeholders::_1);
    _fw->del_run += std::bind(&core::run, this);
    _fw->del_exit += std::bind(&core::exit, this);

}

core::~core(){

}
void core::init(in_cmd* cmd){
     std::printf("run core init() \n");
     for(int i = 0 ; i< cmd->size;i++){

        if(cmd->cmd[i][0] == '-')
            std::printf("cmd:%s \n",cmd->cmd[i]);
     }

     del_loop = std::bind(&game::loop,game::Self());
}


void core::run(){
     std::printf("run core run() in del \n");

     std::printf("enter loop\n");
     loop();
     std::printf("exit loop\n");
}

void core::loop(){
    del_loop();
}

void core::exit(){
     std::printf("run core exit() in del \n");
}