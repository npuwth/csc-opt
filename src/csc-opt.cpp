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
    bool debug;
    int test_num;
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
    
    //1. 参数解析
    std::string b_options;
    std::string o_options;
    bool showhelp = false;
    config.test_num = 1;
    auto cli = lyra::help(showhelp)
        | lyra::opt(b_options, "backend")
            ["--backend"]
            ("which backends to use?")
        | lyra::opt(o_options, "optimization")
            ["--opt"]
            ("which optimizations to use?")
        | lyra::opt(config.test_num, "testnum")
            ["-n"]["--testnum"]
            ("how many times to test?")
        | lyra::opt(config.debug)
            ["-d"]["--debug"]
            ("whether print debug information?")
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
    PASS_DEBUG = config.debug;
    TEST_NUM = config.test_num;

    if(config.i_filename.empty()) {
        printf("Error: No Source File Specified!\n");
        std::cout << cli << "\n";
        return 1;
    } else {
        // fp = fopen(argv[1], "r");
        if(freopen(config.i_filename.c_str(), "r", stdin) == nullptr) {
            printf("Error: Cannot Open Source File!\n");
            std::cout << cli << "\n";
            return 1;
        }
    }
    if(!config.o_filename.empty())
        if(freopen(config.o_filename.c_str(), "w", stdout) == nullptr) {
            printf("Error: Cannot Open Dest File!\n");
            std::cout << cli << "\n";
            return 1;
        }

    //2. 前端解析tac
    parse_tac_file();

    //3. 中间表示cfg
    CFGProgram* cfg = CFGManager::gen_from_program(pg);

    //4. 分析 & 优化
    bool optSSA = 0;
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
            optSSA = 1;
            optimizer.add_pass(PassType::ConvertSSA);
        }
    }

    bool outputSSA = 0;
    for(auto& opt: config.backend) {
        if(opt == "ssa") {
            outputSSA = 1;
        }
    }
    if(!outputSSA && optSSA) {
        optimizer.add_pass(PassType::RevertSSA);
    }
    optimizer.run_pass();

    //5. 后端输出
    for(auto& opt: config.backend) {
        if(opt == "c") {
            pg.toC();
        } else if(opt == "cfg") {
            CFGManager::dump_cfg_program(*cfg, std::cout);
        } else if(opt == "3addr") {
            pg.dump();
        } else if(opt == "rep") { //report
            ;
        } else if(opt == "ssa") {
            // CFGManager::dump_cfg_program(*cfg, std::cout, true);
            ;
        }
    }

    return 0;
}
