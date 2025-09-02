
#pragma once
#include "delegate.h"

#define MAX_CMD 128;

struct in_cmd{
  int size;
  char **cmd;
};

class fw
{
  public:
    fw(int argc , char* argv[]);
    ~fw();
  void init(in_cmd *cmd);  
  
  void run();
  void exit();

  public:
  Delegate<in_cmd*> del_init;
  Delegate<> del_run;
  Delegate<> del_exit;

};