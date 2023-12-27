#ifndef __REVERT_SSA_H__
#define __REVERT_SSA_H__
#include "pass.h"

// 从SSA形式转回原形式
class RevertSSA final :public Pass {
public:
    RevertSSA(CFGProgram* i_cfg) :Pass(i_cfg) {}
    void run();
};

#endif