#include "pass.h"
#include "SCP.h"
#include "DSE.h"
#include "SSA.h"
#include "RSSA.h"
#include "CSE.h"
#include "LICM.h"

bool PASS_DEBUG = false;

void Optimizer::add_pass(PassType pass_type) {
    Pass *pass;
    switch(pass_type) {
        case PassType::SimpleConstantPropagation:
            pass = new SimpleConstantPropagation(m_cfg);
            break;
        case PassType::DeadStatementElimination:
            pass = new DeadStatementElimination(m_cfg);
            break;
        case PassType::ConvertSSA:
            pass = new ConvertSSA(m_cfg);
            break;
        case PassType::RevertSSA:
            pass = new RevertSSA(m_cfg);
            break;
        case PassType::CommonSubexpressionElimination:
            pass = new CommonSubexpressionElimination(m_cfg);
            break;
        case PassType::LoopInvariantCodeMotion:
            pass = new LoopInvariantCodeMotion(m_cfg);
            break;
        default:
            std::cout << "[Error] Wrong pass type." << std::endl;
            break;
    }
    m_passes.push_back(pass);
}

void Optimizer::run_pass() {
    for(auto& pass: m_passes)
        pass->run();
    m_passes.clear();
}
