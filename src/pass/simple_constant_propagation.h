#ifndef __SIMPLE_CONSTANT_PROPAGATION_H__
#define __SIMPLE_CONSTANT_PROPAGATION_H__
#include "pass.h"

// 简单常量传播
class SimpleConstantPropagation final :public Pass
{
public:
    SimpleConstantPropagation(CFGProgram* i_cfg) :Pass(i_cfg) {}
    void run();
};

#endif