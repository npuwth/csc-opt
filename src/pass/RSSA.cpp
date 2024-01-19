#include "RSSA.h"

void RevertSSA::run() {
    if(PASS_DEBUG) {
        std::cout << "INFO: Running RSSA Pass..." << std::endl;
    }
    for(auto& proc: m_cfg->pl) {
        for(auto& blk: proc->get_blocks()) {
            // if(blk->get_index() == 0 || blk->get_index() == 1) continue;
            auto it = blk->get_tac_list().begin();
            if(it == blk->get_tac_list().end()) continue;
            auto pre_tac = (*it)->getPreTac();
            //step1: 删除插入的PHI和MOV
            bool tag = 0;
            for(; it != blk->get_tac_list().end(); ) {
                if((*it)->getOpcode() == Type::PHI) {
                    tag = 1;
                    it = blk->get_tac_list().erase(it); //PHI
                    it = blk->get_tac_list().erase(it); //MOV
                } else {
                    break;
                }
            }
            if(it == blk->get_tac_list().end()) {
                printf("Error: Empty Basic Block Before SSA!\n");
                return;
            }
            if(tag) {
                pre_tac->setSucTac(*it);
                (*it)->setPreTac(pre_tac);
                for(auto& pre: blk->get_pred()) { //更新前驱
                    Tac* tac = pre->get_tac_list().back();
                    if(tac != nullptr) {
                        if(tac->getOpcode() == Type::BR || tac->getOpcode() == Type::CALL) {
                            ((Label*)(tac->getSrc0()))->setPos((*it)->getTacID());
                        } else if(tac->getOpcode() == Type::BLBC || tac->getOpcode() == Type::BLBS) {
                            ((Label*)(tac->getSrc1()))->setPos((*it)->getTacID());
                        }
                    }
                }
            }

            //step2: 重命名变量
            for(auto& tac: blk->get_tac_list()) {
                Operand *src0, *src1;
                src0 = tac->getSrc0();
                src1 = tac->getSrc1();
                if(src0 != nullptr && src0->getType() == OperandType::VAR) {
                    Variable* var = (Variable*)src0;
                    std::string old_name = var->getName().substr(0, var->getName().find("$"));
                    tac->rebindSrc0(proc->get_sym()[old_name]);
                }
                if(src1 != nullptr && src1->getType() == OperandType::VAR) {
                    Variable* var = (Variable*)src1;
                    std::string old_name = var->getName().substr(0, var->getName().find("$"));
                    tac->rebindSrc1(proc->get_sym()[old_name]);
                }
            }
        }
    }
}

