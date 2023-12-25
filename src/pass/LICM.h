#ifndef __LOOP_INVARIANT_CODE_MOTION_H__
#define __LOOP_INVARIANT_CODE_MOTION_H__
#include "pass.h"

// 循环不变代码移动
class LoopInvariantCodeMotion final :public Pass
{
public:
    LoopInvariantCodeMotion(CFGProgram* i_cfg) :Pass(i_cfg) {}
    void run();
};

#endif