#ifndef __DEAD_STATEMENT_ELIMINATION_H__
#define __DEAD_STATEMENT_ELIMINATION_H__
#include "pass.h"
#include "bitmap.h"

// 死代码删除
class DeadStatementElimination final :public Pass
{
private:
    std::vector<Operand*> m_symbols;
    std::vector<BitMap> m_def;
    std::vector<BitMap> m_use;
    std::vector<BitMap> m_in;
    std::vector<BitMap> m_out;
    // 活跃变量
    void initial_symbols(CFGProcedure* proc);
    void compute_def_and_use(CFGProcedure* proc);
    void compute_in_and_out(CFGProcedure* proc);
public:
    DeadStatementElimination(CFGProgram* i_cfg) :Pass(i_cfg) {}
    void run();
};

#endif
