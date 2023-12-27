#include "SCP.h"

void SimpleConstantPropagation::run() {
    std::cout << "INFO: Running SCP Pass..." << std::endl;
    for(auto& proc: *m_cfg) {
        while(true) {
            initial_definitions(proc);
            compute_gen_and_kill(proc);
            compute_in_and_out(proc);
            // print_reaching_definition();
            if(propagate_constant(proc) == 0)
                break;
        }
    }
}

void SimpleConstantPropagation::initial_definitions(CFGProcedure* proc) {
    m_definitions.clear();
    m_gen.clear();
    m_kill.clear();
    m_in.clear();
    m_out.clear();
    for(auto& kv: proc->get_sym()) {
        Operand* symbol = kv.second;
        symbol->def_id.clear();
    }
    int def_id = 0;
    for(auto& blk: proc->get_blocks()) {
        for(auto& tac: blk->get_tac_list()) {
            switch(tac->getDUType()) {  
                //无定值
                case DUType::NDEF:
                    break;
                //MOVE对src1定值的特殊处理
                case DUType::DDEF:
                    assert(tac->getOpcode() == Type::MOVE);
                    if(tac->getSrc0()->getType() == OperandType::CONST)
                        m_definitions.push_back(SCP::Definition(def_id, tac->getSrc1(), tac, true, ((Constant*)(tac->getSrc0()))->num));
                    else
                        m_definitions.push_back(SCP::Definition(def_id, tac->getSrc1(), tac));
                    tac->getSrc1()->def_id.push_back(def_id);
                    def_id++;
                    if(tac->getSrc0()->getType() == OperandType::CONST)
                        m_definitions.push_back(SCP::Definition(def_id, tac->getDest(), tac, true, ((Constant*)(tac->getSrc0()))->num));
                    else
                        m_definitions.push_back(SCP::Definition(def_id, tac->getDest(), tac));
                    tac->getDest()->def_id.push_back(def_id);
                    def_id++;
                    break;
                //对dest定值
                default:
                    m_definitions.push_back(SCP::Definition(def_id, tac->getDest(), tac));
                    tac->getDest()->def_id.push_back(def_id);
                    def_id++;
                    break;
            }
        }
    }
    int blk_num = proc->get_blocks().size();
    int def_num = m_definitions.size();
    for(int i = 0; i < blk_num; i++) {
        m_gen.push_back(BitMap(def_num));
        m_kill.push_back(BitMap(def_num));
        m_in.push_back(BitMap(def_num));
        m_out.push_back(BitMap(def_num));
    }
}

void SimpleConstantPropagation::compute_gen_and_kill(CFGProcedure* proc) {
    for(auto& blk: proc->get_blocks()) {
        int blk_idx = blk->get_index();
        for(auto& tac: blk->get_tac_list()) {
            // gen集
            gen(m_gen[blk_idx], tac, tac->getDest());
            if(tac->getOpcode() == Type::MOVE)
                gen(m_gen[blk_idx], tac, tac->getSrc1());
            // kill集
            kill(m_kill[blk_idx], tac, tac->getDest());
            if(tac->getOpcode() == Type::MOVE)
                kill(m_kill[blk_idx], tac, tac->getSrc1());   
        }
    }
}

void SimpleConstantPropagation::compute_in_and_out(CFGProcedure* proc) {
    std::queue<CFGBlock*> q;
    for(auto& block: proc->get_blocks())
        q.push(block);
    while(!q.empty()) {
        CFGBlock* current_block = q.front();
        q.pop();
        int blk_idx = current_block->get_index();
        BitMap old_value = m_out[blk_idx];
        // IN[B]=U(P,pred)OUT[P]
        for(auto it = current_block->get_pred().begin(); it != current_block->get_pred().end(); it++)
            if(*it != nullptr && (*it)->get_index() >= 0)
                m_in[blk_idx] |= m_out[(*it)->get_index()];
        // OUT[B] = gen[B] U (IN[B] - kill[B])
        m_out[blk_idx] = m_gen[blk_idx] | (m_in[blk_idx] - m_kill[blk_idx]);
        old_value ^= m_out[blk_idx];    //异或判断是否相同
        if(!old_value.empty())
            for(auto it = current_block->get_succ().begin(); it != current_block->get_succ().end(); it++)
                if(*it != nullptr && (*it)->get_index() >= 0)
                    q.push(*it);
    }
}

void SimpleConstantPropagation::gen(BitMap& gen, Tac* tac, Operand* oper) {
    for(auto& def_id: oper->def_id) {
        assert(m_definitions[def_id].id == def_id);
        assert(m_definitions[def_id].dest == oper);
        //gen该定值
        if(m_definitions[def_id].tac == tac) {
            gen.set(def_id);
            // std::cout << "gen set d" << def_id << std::endl;
        }
        else {
            gen.reset(def_id);
            // std::cout << "gen reset d" << def_id << std::endl;
        }
    }
}

void SimpleConstantPropagation::kill(BitMap& kill, Tac* tac, Operand* oper) {
    for(auto& def_id: oper->def_id) {
        assert(m_definitions[def_id].id == def_id);
        assert(m_definitions[def_id].dest == oper);
        //kill所有除该定值以外的所有定值
        if(m_definitions[def_id].tac == tac)
            continue;
        kill.set(def_id);
        // std::cout << "kill d" << def_id << std::endl;
    }
}

void SimpleConstantPropagation::print_reaching_definition() {
    std::cout << "---Reaching Definition---" << std::endl;
    for(auto& def: m_definitions) {
        std::cout << "d" << def.id << ": ";
        def.dest->dump();
        // std::cout << " at " << def.dest;
        if(def.is_const)
            std::cout << " Const " << def.const_value;
        def.tac->dump();
    }
    for(int i = 0; i < (int)(m_gen.size()); i++) {
        std::cout << "block[" << i << "]" << std::endl;
        std::cout << "  gen  = " << m_gen[i].get_string() << std::endl;
        std::cout << "  kill = " << m_kill[i].get_string() << std::endl;
        std::cout << "  in   = " << m_in[i].get_string() << std::endl;
        std::cout << "  out  = " << m_out[i].get_string() << std::endl;
    }
    std::cout << "-------------------------" << std::endl;
}

bool SimpleConstantPropagation::check(Operand* oper, BitMap& def_in, int* definition_id) {
    int def_num = 0;
    int sole_def_id = -1;
    if(oper == nullptr)
        return false;
    for(auto& def_id: oper->def_id)
        if(def_in.test(def_id)) {
            def_num++;
            sole_def_id = def_id;
        }
    if(def_num == 1) {   //只有唯一一个定值到达这里
        assert(sole_def_id >= 0);
        if(m_definitions[sole_def_id].is_const) { //该定值为常量
            *definition_id = sole_def_id;
            return true;
        }
    }
    return false;
}

void SimpleConstantPropagation::replace(Tac* tac, Operand* oper, int definition_id) {
    assert(definition_id >= 0);
    std::cout << "instr " << tac->getTacID() << ": replace ";
    oper->dump();
    std::cout << " with d" << m_definitions[definition_id].id << ": const " << m_definitions[definition_id].const_value << std::endl;
    if(tac->getSrc0() == oper)
        tac->rebindSrc0(new Constant(m_definitions[definition_id].const_value));
    else if(tac->getSrc1() == oper)
        tac->rebindSrc1(new Constant(m_definitions[definition_id].const_value));
}

int SimpleConstantPropagation::propagate_constant(CFGProcedure* proc) {
    int replace_cnt = 0;
    for(auto& blk: proc->get_blocks()) {
        int blk_idx = blk->get_index();
        BitMap def_in = BitMap(m_in[blk_idx]);
        for(auto& tac: blk->get_tac_list()) {
            int definition_id = -1;
            //判断常量传播
            if(check(tac->getSrc0(), def_in, &definition_id)) {
                replace(tac, tac->getSrc0(), definition_id);
                replace_cnt++;
            }
            if(tac->getOpcode() != Type::MOVE)
                if(check(tac->getSrc1(), def_in, &definition_id)) {
                    replace(tac, tac->getSrc1(), definition_id);
                    replace_cnt++;
                }
            //按指令更新def_in
            gen(def_in, tac, tac->getDest());
            if(tac->getOpcode() == Type::MOVE)
                gen(def_in, tac, tac->getSrc1());
        }
    }
    return replace_cnt;
}
