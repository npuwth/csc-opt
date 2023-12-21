#ifndef __SIMPLE_CONSTANT_PROPAGATION_H__
#define __SIMPLE_CONSTANT_PROPAGATION_H__
#include "pass.h"
#include "bitmap.h"
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
public:
    SimpleConstantPropagation(CFGProgram* i_cfg) :Pass(i_cfg) {}
    // 到达定值
    void initial_definitions(CFGProcedure* proc);
    void compute_gen_and_kill();
    void compute_in_and_out();
    void run();
};

#endif