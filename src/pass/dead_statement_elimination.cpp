#include "dead_statement_elimination.h"

void DeadStatementElimination::run() {
    std::cout << "running DSE pass..." << std::endl;
    for(auto& proc: *m_cfg)
    {
        initial_symbols(proc);
        compute_def_and_use(proc);
        compute_in_and_out(proc);
    }
}


void DeadStatementElimination::initial_symbols(CFGProcedure* proc)
{

}
void DeadStatementElimination::compute_def_and_use(CFGProcedure* proc)
{

}
void DeadStatementElimination::compute_in_and_out(CFGProcedure* proc)
{

}