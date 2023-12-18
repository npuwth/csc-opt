#include<stdio.h>
#include<iostream>
#include<string>
#include"tac.h"
#include"cfg.h"
#include"pass.h"

int main(int argc, char *argv[]) {
    
    if(argc < 2) {
        printf("Usage: ./csc-opt xxx.tac\n");
        return 1;
    } else {
        // fp = fopen(argv[1], "r");
        freopen(argv[1], "r", stdin);
    }

    // 前端
    parse_tac_file();
    std::cout << "------TAC------" << std::endl;
    // pg.dump();
    std::cout << "---------------" << std::endl;

    // 中间表示
    CFGProgram* cfg = CFGManager::gen_from_program(pg);
    std::cout << "------simple CFG------" << std::endl;
    CFGManager::dump_cfg_program(*cfg, std::cout);
    std::cout << "----------------------" << std::endl;

    // 分析与优化
    Optimizer optimizer = Optimizer(cfg);
    // lab2
    optimizer.add_pass(PassType::SimpleConstantPropagation);
    optimizer.add_pass(PassType::DeadStatementElimination);
    optimizer.run_pass();
    // lab3
    optimizer.add_pass(PassType::ConvertSSA);
    optimizer.add_pass(PassType::CommonSubexpressionElimination);
    optimizer.add_pass(PassType::LoopInvariantCodeMotion);
    optimizer.add_pass(PassType::RevertSSA);
    optimizer.run_pass();

    // 后端
    std::cout << "------complete CFG------" << std::endl;
    // CFGManager::dump_cfg_program(*cfg, std::cout, true);
    std::cout << "------------------------" << std::endl;

    return 0;
}
