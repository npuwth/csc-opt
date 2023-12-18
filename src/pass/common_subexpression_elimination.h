#ifndef __COMMON_SUBEXPRESSION_ELIMINATION_H__
#define __COMMON_SUBEXPRESSION_ELIMINATION_H__
#include "pass.h"

// 公共子表达式删除
class CommonSubexpressionElimination final :public Pass
{
public:
    CommonSubexpressionElimination(CFGProgram* i_cfg) :Pass(i_cfg) {}
    void run();
};

#endif