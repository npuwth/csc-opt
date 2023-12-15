#include "pass.h"

void SimpleConstantPropagation::run() {

}

void DeadStatementElimination::run() {

}

void ConvertSSA::run() {

}

void RevertSSA::run() {

}

void CommonSubexpressionElimination::run() {

}

void LoopInvariantCodeMotion::run() {

}

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
}