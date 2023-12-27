#include "DSE.h"

void DeadStatementElimination::run() {
    std::cout << "INFO: Running DSE Pass..." << std::endl;
    for(auto& proc: *m_cfg) {
        while(true) {
            initial_symbols(proc);
            compute_def_and_use(proc);
            compute_in_and_out(proc);
            // print_living_variable();
            if(eliminate_dead_statement(proc) == 0) //到达不动点
                break;
        }
    }
}

void DeadStatementElimination::initial_symbols(CFGProcedure* proc) {
    m_symbols.clear();
    m_def.clear();
    m_use.clear();
    m_in.clear();
    m_out.clear();

    int sym_idx = 0;
    for(auto& kv: proc->get_sym()) {
        Operand* symbol = kv.second;
        if(symbol->getType() == OperandType::REG || symbol->getType() == OperandType::VAR) {
            // std::cout << "add ";
            // symbol->dump();
            // std::cout << std::endl;
            symbol->oper_id = sym_idx;
            m_symbols.push_back(symbol);
            sym_idx++;
        }
    }
    int blk_num = proc->get_blocks().size();
    int sym_num = m_symbols.size();
    for(int i = 0; i < blk_num; i++) {
        m_def.push_back(BitMap(sym_num));
        m_use.push_back(BitMap(sym_num));
        m_in.push_back(BitMap(sym_num));
        m_out.push_back(BitMap(sym_num));
    }

}

void DeadStatementElimination::compute_def_and_use(CFGProcedure* proc) {
    for(auto& blk: proc->get_blocks()) {
        int blk_idx = blk->get_index();
        for(auto& tac: blk->get_tac_list()) {
            switch(tac->getDUType()) {
                //无定值
                case DUType::NDEF:
                    use(m_use[blk_idx], m_def[blk_idx], tac->getSrc0());
                    use(m_use[blk_idx], m_def[blk_idx], tac->getSrc1());
                    break;
                //MOVE对src1定值的特殊处理
                case DUType::DDEF:
                    use(m_use[blk_idx], m_def[blk_idx], tac->getSrc0());
                    def(m_def[blk_idx], tac->getSrc1());
                    def(m_def[blk_idx], tac->getDest());
                    break;
                default:
                    use(m_use[blk_idx], m_def[blk_idx], tac->getSrc0());
                    use(m_use[blk_idx], m_def[blk_idx], tac->getSrc1());
                    def(m_def[blk_idx], tac->getDest());
                    break;
            } 
        }
    }
}

void DeadStatementElimination::compute_in_and_out(CFGProcedure* proc) {
    std::queue<CFGBlock*> q;
    for(auto it = proc->get_blocks().rbegin(); it != proc->get_blocks().rend(); it++)
        q.push(*it);
    // for(auto& block: proc->get_blocks())
    //     q.push(block);
    while(!q.empty()) {
        CFGBlock* current_block = q.front();
        q.pop();
        int blk_idx = current_block->get_index();
        BitMap old_value = m_in[blk_idx];
        // OUT[B]=U(S,succ)IN[S]
        for(auto it = current_block->get_succ().begin(); it != current_block->get_succ().end(); it++)
            if(*it != nullptr && (*it)->get_index() >= 0)
                m_out[blk_idx] |= m_in[(*it)->get_index()];
        // IN[B] = use[B] U (OUT[B] - def[B])
        m_in[blk_idx] = m_use[blk_idx] | (m_out[blk_idx] - m_def[blk_idx]);
        old_value ^= m_in[blk_idx];    //异或判断是否相同
        if(!old_value.empty())
            for(auto it = current_block->get_pred().begin(); it != current_block->get_pred().end(); it++)
                if(*it != nullptr && (*it)->get_index() >= 0)
                    q.push(*it);
    }
}

void DeadStatementElimination::def(BitMap& def, Operand* oper) {
    if(oper != nullptr && oper->oper_id >= 0)
        def.set(oper->oper_id);
}

void DeadStatementElimination::use(BitMap& use, BitMap& def, Operand* oper) {
    if(oper != nullptr && oper->oper_id >=0 && !def.test(oper->oper_id))
        use.set(oper->oper_id);
}

void DeadStatementElimination::print_living_variable() {
    std::cout << "-----Living Variable-----" << std::endl;
    for(auto& sym: m_symbols) {
        std::cout << "symbol " << sym->oper_id << ": ";
        sym->dump();
        std::cout << std::endl;
    }
    for(int i = 0; i < (int)(m_def.size()); i++) {
        std::cout << "block[" << i << "]" << std::endl;
        std::cout << "  def  = " << m_def[i].get_string() << std::endl;
        std::cout << "  use  = " << m_use[i].get_string() << std::endl;
        std::cout << "  in   = " << m_in[i].get_string() << std::endl;
        std::cout << "  out  = " << m_out[i].get_string() << std::endl;
    }
    std::cout << "-------------------------" << std::endl;
}

bool DeadStatementElimination::check(Operand* oper, BitMap& live_out) {
    if(!live_out.test(oper->oper_id))   //不活跃则可以删除
        return true;
    return false;
}

void DeadStatementElimination::eliminate(Tac** tac) {
    std::cout << "eliminate ";
    (*tac)->dump();
    (*tac)->rebindOp(Type::NOP, "nop");
    (*tac)->rebindSrc0(nullptr);
    (*tac)->rebindSrc1(nullptr);
}

void DeadStatementElimination::remove_nop(CFGBlock* blk) {
    for(auto it = blk->get_tac_list().begin(); it != blk->get_tac_list().end(); it++) {
        Tac* now_tac = *it;
        if(now_tac->getOpcode() == Type::NOP) {
            Tac* pre_tac = now_tac->getPreTac();
            Tac* suc_tac = now_tac->getSucTac();
            suc_tac->setPreTac(pre_tac);
            pre_tac->setSucTac(suc_tac);
            now_tac->setPreTac(nullptr);
            now_tac->setSucTac(nullptr);
            it = blk->get_tac_list().erase(it);
            it--;
            delete(now_tac);
        }
    }
}

int DeadStatementElimination::eliminate_dead_statement(CFGProcedure* proc) {
    int eliminate_cnt = 0;
    for(auto& blk: proc->get_blocks()) {
        int blk_idx = blk->get_index();
        BitMap live_out = BitMap(m_out[blk_idx]);
        //逆序遍历指令
        for(auto it = blk->get_tac_list().rbegin(); it != blk->get_tac_list().rend(); it++) {
            Tac* tac = *it;
            switch(tac->getDUType()) {
                //无定值
                case DUType::NDEF:
                    //更新live_out
                    if(tac->getSrc0() != nullptr && tac->getSrc0()->oper_id >= 0)
                        live_out.set(tac->getSrc0()->oper_id);
                    if(tac->getSrc1() != nullptr && tac->getSrc1()->oper_id >= 0)
                        live_out.set(tac->getSrc1()->oper_id);
                    break;
                //MOVE对src1定值的特殊处理
                case DUType::DDEF:
                    if(check(tac->getSrc1(), live_out) && check(tac->getDest(), live_out)) {
                        eliminate(&tac);
                        eliminate_cnt++;
                    }
                    else {
                        //更新live_out
                        if(tac->getDest() != nullptr && tac->getDest()->oper_id >= 0)
                            live_out.reset(tac->getDest()->oper_id);
                        if(tac->getSrc1() != nullptr && tac->getSrc1()->oper_id >= 0)
                            live_out.reset(tac->getSrc1()->oper_id);
                        if(tac->getSrc0() != nullptr && tac->getSrc0()->oper_id >= 0)
                            live_out.set(tac->getSrc0()->oper_id);
                    }
                    break;
                default:
                    // READ作特殊处理，即使READ的定值未使用也不可以删除
                    if(check(tac->getDest(), live_out) && tac->getOpcode() != Type::READ) {
                        eliminate(&tac);
                        eliminate_cnt++;
                    }
                    else {
                        //更新live_out
                        if(tac->getDest() != nullptr && tac->getDest()->oper_id >= 0)
                            live_out.reset(tac->getDest()->oper_id);
                        if(tac->getSrc0() != nullptr && tac->getSrc0()->oper_id >= 0)
                            live_out.set(tac->getSrc0()->oper_id);
                        if(tac->getSrc1() != nullptr && tac->getSrc1()->oper_id >= 0)
                            live_out.set(tac->getSrc1()->oper_id);
                    }
                    break;
            } 
        }
        remove_nop(blk);
    }
    return eliminate_cnt;
}
