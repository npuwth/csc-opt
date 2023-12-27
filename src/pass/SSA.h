#ifndef __CONVERT_SSA_H__
#define __CONVERT_SSA_H__
#include "pass.h"

// 转为SSA形式
class ConvertSSA final :public Pass {
private:
    std::vector<int> postOrderID; // idx -> postOrder
    std::vector<int> reversePostOrder; // reversePostOrder -> idx
private:
    void compute_reversePostOrder(CFGProcedure* proc);
    void compute_immediate_dominator(CFGProcedure* proc);
    void compute_dominance_frontiers(CFGProcedure* proc);
    void insert_phi_functions(CFGProcedure* proc);
    void rename_variables(CFGProcedure* proc);
    CFGBlock* intersect(CFGBlock* x, CFGBlock* y);
public:
    ConvertSSA(CFGProgram* i_cfg) :Pass(i_cfg) {}
    void run();
};

#endif