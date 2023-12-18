#include "pass.h"
#include "simple_constant_propagation.h"
#include "dead_statement_elimination.h"
#include "convert_ssa.h"
#include "revert_ssa.h"
#include "common_subexpression_elimination.h"
#include "loop_invariant_code_motion.h"

void Optimizer::add_pass(PassType pass_type)
{
    Pass *pass;
    switch(pass_type)
    {
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

void Optimizer::run_pass()
{
    for(auto& pass: m_passes)
        pass->run();
    m_passes.clear();
}