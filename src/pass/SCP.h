#ifndef __SIMPLE_CONSTANT_PROPAGATION_H__
#define __SIMPLE_CONSTANT_PROPAGATION_H__
#include "pass.h"
#include "bitmap.h"
#include <cassert>

namespace SCP {
    struct Definition {
        int id;
        Operand* dest;
        Tac* tac;
        bool is_const;
        dt_ulong const_value;
        Definition(int id, Operand* dest, Tac* tac, bool is_const = false, dt_ulong const_value = 0):
            id(id), dest(dest), tac(tac), is_const(is_const), const_value(const_value) {}
    };
}
// 简单常量传播
class SimpleConstantPropagation final :public Pass {
private:
    std::vector<SCP::Definition> m_definitions;
    std::vector<BitMap> m_gen;
    std::vector<BitMap> m_kill;
    std::vector<BitMap> m_in;
    std::vector<BitMap> m_out;
    // 到达定值分析
    void initial_definitions(CFGProcedure* proc);
    void compute_gen_and_kill(CFGProcedure* proc);
    void compute_in_and_out(CFGProcedure* proc);
    void gen(BitMap& gen, Tac* tac, Operand* oper);
    void kill(BitMap& kill, Tac* tac, Operand* oper);
    void print_reaching_definition();
    // 常量传播
    dt_ulong calculate_const(Tac* tac);
    bool check(Operand* oper, BitMap& def_in, int* definition_id);
    void replace(Tac* tac, Operand* oper, int definition_id);
    int propagate_constant(CFGProcedure* proc);
public:
    SimpleConstantPropagation(CFGProgram* i_cfg) :Pass(i_cfg) {}
    void run();
};

#endif