#include "RSSA.h"

void RevertSSA::run() {
    std::cout << "INFO: Running RSSA Pass..." << std::endl;
    for(auto& proc: m_cfg->pl) {
        for(auto& blk: proc->get_blocks()) {
            auto it = blk->get_tac_list().begin();
            // for(; it != blk->get_tac_list().end(); ) {
            // Step1: 删除加入的PHI和MOV
            //     if(tac->getOpcode() == Type::PHI) {

            //     }
            // Step2: 重命名变量
            // }
        }
    }
}

