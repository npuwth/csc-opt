#ifndef __CONVERT_SSA_H__
#define __CONVERT_SSA_H__
#include "pass.h"

// 转为SSA形式
class ConvertSSA final :public Pass {
private:
    void compute_immediate_dominator();
    void compute_dominance_frontiers();
    void insert_phi_functions();
    void rename_variables();
public:
    ConvertSSA(CFGProgram* i_cfg) :Pass(i_cfg) {}
    void run();
};

#endif