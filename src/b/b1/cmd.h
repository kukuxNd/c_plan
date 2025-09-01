#pragma once
#include <string>

struct cmd_head
{
    std::string cmd_text;
    void* pdata;
    /* data */
    cmd_head* next; 
};

extern bool cmd_loop();
extern void cmd_add_task(std::string txt);
extern void register_cmd(std::string cmd, std::function<void()> func);
extern std::string last_cmd_txt;