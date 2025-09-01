#include "game.h"

game* game::instance = nullptr;

void gLog(const std::string& msg){
    game::getInstance()->AddLog(msg);
}

void cmd_a(){
    gLog("执行命令 a");
}

void cmd_b(){
      gLog("执行命令 b");

}

void cmd_c(){
    gLog("执行命令 c");
}

void do_reg_cmd(){
    register_cmd("a", cmd_a);
    register_cmd("b", cmd_b);
    register_cmd("c", cmd_c);
    register_cmd("w", [](){
        gLog("按了向上的 方向按键");
    });
}



int main(){
    printf("start main\n");

    do_reg_cmd();
    game g;
    g.init();
    g.run();
    return 0;
}