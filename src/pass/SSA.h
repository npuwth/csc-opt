#ifndef __CONVERT_SSA_H__
#define __CONVERT_SSA_H__
#include "pass.h"

// 转为SSA形式
class ConvertSSA final :public Pass {
private:
    std::vector<int> postOrderID; // idx -> postOrder
    std::vector<int> reversePostOrder; // reversePostOrder -> idx
    std::vector<bool> visited;
    dt_long cmaxTacID;
    int varID = 0;
    std::unordered_map<Variable*, int> varMap;   //old_var -> idx
    std::vector<int> varCount;                   //idx -> count
    std::vector<std::stack<Variable*>> varStack; //idx -> stack
    std::set<CFGBlock*> hasAlready;
    std::set<CFGBlock*> everOnWorkList;
    std::queue<CFGBlock*> workList;
    CFGProcedure* curProc;
private:
    void compute_reversePostOrder(CFGProcedure* proc);
    void compute_immediate_dominator(CFGProcedure* proc);
    void compute_dominance_frontiers(CFGProcedure* proc);
    void insert_phi_functions(CFGProcedure* proc);
    Variable* gen_name(Variable* var);
    void rename_variables(CFGBlock* blk);
    CFGBlock* intersect(CFGBlock* x, CFGBlock* y);
public:
    ConvertSSA(CFGProgram* i_cfg) :Pass(i_cfg) {}
    void run();
};

#endif