#include <string>
#include <unordered_map>
#include <functional>
#include "cmd.h"


void (*cmd_func)();

cmd_head *cmd_task;

void cmd_add_task(std::string txt){
    cmd_head* pCmd = new cmd_head();
    pCmd->cmd_text = txt;
    if(cmd_task != nullptr)
        cmd_task->next = pCmd;
    else
     cmd_task = pCmd;
}

std::unordered_map<std::string, std::function<void()>> cmd_func_list;

void register_cmd(std::string cmd, std::function<void()> func){
    cmd_func_list[cmd] = func;
}

std::string last_cmd_txt;
bool cmd_loop()
{ 
    if(cmd_task == nullptr)
        return true; // 没有任务，直接返回true

    while(cmd_task != nullptr){
        auto it = cmd_func_list.find(cmd_task->cmd_text);
        last_cmd_txt = cmd_task->cmd_text;

        if(it != cmd_func_list.end()){
            it->second(); // 调用对应的函数
        } else {
            cmd_head* temp = cmd_task;
     
            cmd_task = cmd_task->next;
            delete temp; // 释放内存
           return false; // 未找到命令，返回false
        }
        cmd_head* temp = cmd_task;
        cmd_task = cmd_task->next;
        delete temp; // 释放内存
        
    }

    return true;
}