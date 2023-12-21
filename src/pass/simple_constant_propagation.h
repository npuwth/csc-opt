#ifndef __SIMPLE_CONSTANT_PROPAGATION_H__
#define __SIMPLE_CONSTANT_PROPAGATION_H__
#include "pass.h"
#include "bitmap.h"
#include <cassert>

namespace SCP
{
    struct Definition{
        int id;
        Operand* dest;
        Tac* tac;
        Definition(int id, Operand* dest, Tac* tac): id(id), dest(dest), tac(tac) {}
    };
}
// 简单常量传播
class SimpleConstantPropagation final :public Pass
{
private:
    std::vector<SCP::Definition> m_definitions;
    std::vector<BitMap> m_gen;
    std::vector<BitMap> m_kill;
    std::vector<BitMap> m_in;
    std::vector<BitMap> m_out;
    // 到达定值
    void initial_definitions(CFGProcedure* proc);
    void compute_gen_and_kill(CFGProcedure* proc);
    void compute_in_and_out(CFGProcedure* proc);
    void gen(int blk_idx, Tac* tac, Operand* oper);
    void kill(int blk_idx, Tac* tac, Operand* oper);
    void print_reaching_definition();
public:
    SimpleConstantPropagation(CFGProgram* i_cfg) :Pass(i_cfg) {}
    void run();
};

#endif