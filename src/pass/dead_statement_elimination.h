#ifndef __DEAD_STATEMENT_ELIMINATION_H__
#define __DEAD_STATEMENT_ELIMINATION_H__
#include "pass.h"

// 死代码删除
class DeadStatementElimination final :public Pass
{
public:
    DeadStatementElimination(CFGProgram* i_cfg) :Pass(i_cfg) {}
    void run();
};

#endif
