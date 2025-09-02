
#include "fw/fw.h"


fw* g_fw = nullptr;

int main(int argc, char *argv[]) {
    g_fw = new fw(argc,argv);
    return 0;
}