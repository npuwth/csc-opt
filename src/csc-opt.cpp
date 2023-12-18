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

    parse_tac_file();
    std::cout << "------TAC------" << std::endl;
    pg.dump();
    std::cout << "---------------" << std::endl;

    CFGProgram* cfg = CFGManager::gen_from_program(pg);

    std::cout << "------CFG------" << std::endl;
    CFGManager::dump_cfg_program(*cfg, std::cout);
    std::cout << "---------------" << std::endl;

    return 0;
}
