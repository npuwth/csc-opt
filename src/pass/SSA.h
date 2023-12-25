#ifndef __CONVERT_SSA_H__
#define __CONVERT_SSA_H__
#include "pass.h"

// 转为SSA形式
class ConvertSSA final :public Pass
{
public:
    ConvertSSA(CFGProgram* i_cfg) :Pass(i_cfg) {}
    void run();
};

#endif