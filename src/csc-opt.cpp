#include<stdio.h>
#include<iostream>
#include<string>
#include<vector>
#include"lyra.hpp"
#include"tac.h"
#include"cfg.h"
#include"pass.h"

struct Config {
    std::vector<std::string> backend;
    std::vector<std::string> optimization;
    std::string filename;
} config;

std::vector<std::string> split_string(const std::string& s, char seperator) {
    std::vector<std::string> output;
    std::string::size_type prev_pos = 0, pos = 0;
    while((pos = s.find(seperator, pos)) != std::string::npos) {
        std::string substring( s.substr(prev_pos, pos-prev_pos) );
        output.push_back(substring);
        prev_pos = ++pos;
    }
    output.push_back(s.substr(prev_pos, pos-prev_pos)); // Last word
    return output;
}

int main(int argc, char *argv[]) {
    
    // if(argc < 2) {
    //     printf("Usage: ./csc-opt xxx.tac\n");
    //     return 1;
    // } else {
    //     // fp = fopen(argv[1], "r");
    //     freopen(argv[1], "r", stdin);
    // }

    //参数解析
    std::string b_options;
    std::string o_options;
    bool showhelp = false;
    auto cli = lyra::help(showhelp)
        | lyra::opt(b_options, "backend")
            ["--backend"]
            ("which backends to use?")
        | lyra::opt(o_options, "optimization")
            ["--opt"]
            ("which optimizations to use?")
        | lyra::arg(config.filename, "source file")
            ("which source file to use?");

    auto result = cli.parse({argc, argv});
    if(!result) {
        printf("Error: Cannot Parse Cmd Arguments!\n");
        std::cout << cli << "\n";
        return 1;
    }
    if(showhelp) {
        std::cout << cli << "\n";
        return 0;
    }
    config.backend = split_string(b_options, ',');
    config.optimization = split_string(o_options, ',');

    //debug
    printf("Backends:\n");
    for(auto it = config.backend.begin(); it != config.backend.end(); it++) {
        printf("\t%s\n", it->c_str());
    }
    printf("Optimizations:\n");
    for(auto it = config.optimization.begin(); it != config.optimization.end(); it++) {
        printf("\t%s\n", it->c_str());
    }
    printf("Source File:\n");
    printf("\t%s\n", config.filename.c_str());

    freopen(config.filename.c_str(), "r", stdin);

    // 前端
    parse_tac_file();
    std::cout << "------TAC------" << std::endl;
    pg.dump();
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
