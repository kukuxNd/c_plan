#pragma once

#include "fw.h"

typedef void (*LoopFunc) ();

class core
{
    public:
        core(fw* _fw);
        ~core();
   void init(in_cmd* cmd);
   void run();
   void exit();
   void loop();
   fw* m_pfw;

   void reg_init( Delegate<in_cmd*> _d_init);
   void reg_loop();
   private:
   std::function<void()> del_loop;
};