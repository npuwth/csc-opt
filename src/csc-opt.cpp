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
    std::string i_filename;
    std::string o_filename;
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
    
    // 参数解析
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
        | lyra::opt(config.o_filename, "destFile")
            ["-o"]["--output"]
            ("which dest file to use?")
        | lyra::arg(config.i_filename, "sourceFile")
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

    if(config.i_filename.empty()) {
        printf("Error: No Source File Specified!\n");
        return 1;
    } else {
        // fp = fopen(argv[1], "r");
        if(freopen(config.i_filename.c_str(), "r", stdin) == nullptr) {
            printf("Error: Cannot Open Source File!\n");
            return 1;
        }
    }
    if(!config.o_filename.empty())
        if(freopen(config.o_filename.c_str(), "w", stdout) == nullptr) {
            printf("Error: Cannot Open Dest File!\n");
            return 1;
        }

    // 前端
    parse_tac_file();

    // 中间表示cfg
    CFGProgram* cfg = CFGManager::gen_from_program(pg);

    // 分析与优化
    Optimizer optimizer = Optimizer(cfg);
    for(auto& opt: config.optimization) {
        if(opt == "scp") {
            optimizer.add_pass(PassType::SimpleConstantPropagation);
        } else if(opt == "cse") {
            optimizer.add_pass(PassType::CommonSubexpressionElimination);
        } else if(opt == "dse") {
            optimizer.add_pass(PassType::DeadStatementElimination);
        } else if(opt == "licm") {
            optimizer.add_pass(PassType::LoopInvariantCodeMotion);
        } else if(opt == "ssa") {
            optimizer.add_pass(PassType::ConvertSSA);
        }
    }
    // optimizer.add_pass(PassType::RevertSSA); //TODO
    optimizer.run_pass();

    // 后端
    for(auto& opt: config.backend) {
        if(opt == "c") {
            ;
        } else if(opt == "cfg") {
            std::cout << "------CFG------" << std::endl;
            CFGManager::dump_cfg_program(*cfg, std::cout);
            std::cout << "---------------" << std::endl;
        } else if(opt == "3addr") {
            std::cout << "------TAC------" << std::endl;
            pg.dump();
            std::cout << "---------------" << std::endl;
        } else if(opt == "rep") { //report
            ;
        } else if(opt == "ssa") {
            ;
        }
    }

    return 0;
}
