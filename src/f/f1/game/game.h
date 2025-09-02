#pragma once
#include "fw/core.h"

#include <windows.h>  // 

#define usleep(microseconds) Sleep((microseconds + 999) / 1000)  // 微秒转毫秒（四舍五入）


class igame{
    public:
    virtual void init() = 0;
    virtual void start() = 0;
    virtual void loop() = 0;
    virtual void end() = 0;
};
enum Gamestate
{   
    OUT_WORLD,
    IN_WORLD,
        
};


class game : public igame
{
    private:
    static game* self;

    public:
    
    static game* Self(){
        if(self == nullptr)
           self =  new game();
        return self;
    }

    Gamestate state = OUT_WORLD;
    game();
    ~game();

    void init() override;
    void start() override;
    void loop() override;
    void end() override;
};