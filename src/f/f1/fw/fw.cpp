#include "fw.h"
#include "core.h"

fw::fw(int argc , char* argv[]){
    in_cmd cmd ;
    cmd.size = argc;
    cmd.cmd = argv;
    core _core(this);

    init(&cmd);
    run();
    exit();
}

fw::~fw(){

}

void fw::init(in_cmd *cmd){
    del_init(cmd);
}

void fw::run(){
    del_run();
}

void fw::exit()
{
    del_exit();
}