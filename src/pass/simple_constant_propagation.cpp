#include "simple_constant_propagation.h"

void SimpleConstantPropagation::run() {
    std::cout << "running SCP pass..." << std::endl;
    for(auto& proc: *m_cfg)
    {
        initial_definitions(proc);
        // compute_gen_and_kill(proc);
        // compute_in_and_out(proc);
        print_reaching_definition();
    }
}

void SimpleConstantPropagation::initial_definitions(CFGProcedure* proc)
{
    m_definitions.clear();
    m_gen.clear();
    m_kill.clear();
    m_in.clear();
    m_out.clear();
    int def_id = 0;
    for(auto& blk: proc->get_blocks())
    {
        for(auto& tac: blk->get_tac_list())
        {
            switch(tac->getOpcode())
            {  
                //无定值
                case Type::BR: case Type::BLBC: case Type::BLBS: case Type::CALL:
                case Type::STORE: case Type::WRITE: case Type::WRL: case Type::PARAM:
                case Type::ENTER: case Type::RET: case Type::NOP: case Type::ENTRYPC:
                    break;
                //MOVE对src1定值的特殊处理
                case Type::MOVE:
                    m_definitions.push_back(SCP::Definition(def_id, tac->getSrc1(), tac));
                    tac->getSrc1()->def_id.push_back(def_id);
                    def_id++;
                //对dest定值
                default:
                    m_definitions.push_back(SCP::Definition(def_id, tac->getDest(), tac));
                    tac->getDest()->def_id.push_back(def_id);
                    def_id++;
                    break;
            }
        }
    }
    int blk_num = proc->get_blocks().size();    //再加上ENTRY和EXIT这两个
    int def_num = m_definitions.size();
    for(int i = 0; i < blk_num; i++)
    {
        m_gen.push_back(BitMap(def_num));
        m_kill.push_back(BitMap(def_num));
        m_in.push_back(BitMap(def_num));
        m_out.push_back(BitMap(def_num));
    }
}
void SimpleConstantPropagation::compute_gen_and_kill(CFGProcedure* proc)
{
    for(auto& blk: proc->get_blocks())
    {
        int blk_idx = blk->get_index();
        for(auto& tac: blk->get_tac_list())
        {
            // gen集
            gen(blk_idx, tac, tac->getDest());
            if(tac->getOpcode() == Type::MOVE)
                gen(blk_idx, tac, tac->getSrc1());
            // kill集
            kill(blk_idx, tac, tac->getDest());
            if(tac->getOpcode() == Type::MOVE)
                kill(blk_idx, tac, tac->getSrc1());   
        }
    }
}
void SimpleConstantPropagation::compute_in_and_out(CFGProcedure* proc)
{
        std::cout << "2" << std::endl;
    std::queue<CFGBlock*> q;
    q.push(proc->get_entry());
    while(!q.empty())
    {
        CFGBlock* current_block = q.front();
        q.pop();
        int blk_idx = current_block->get_index();
        BitMap old_value = m_out[blk_idx];
        // IN[B]=U(P,pred)OUT[P]
        for(auto it = current_block->get_pred().begin(); it != current_block->get_pred().end(); it++)
            m_in[blk_idx] |= m_out[(*it)->get_index()];
        // OUT[B] = gen[B] U (IN[B] - kill[B])
        m_out[blk_idx] = m_gen[blk_idx] | (m_in[blk_idx] - m_kill[blk_idx]);
        old_value ^= m_out[blk_idx];    //异或判断是否相同
        if(!old_value.empty())
            for(auto it = current_block->get_succ().begin(); it != current_block->get_succ().end(); it++)
                if(*it != nullptr)
                    q.push(*it);
    }
}

void SimpleConstantPropagation::gen(int blk_idx, Tac* tac, Operand* oper)
{
    for(auto& def_id: oper->def_id)
    {
        assert(m_definitions[def_id].id == def_id);
        assert(m_definitions[def_id].dest == oper);
        //gen该定值
        if(m_definitions[def_id].tac == tac)
            m_gen[blk_idx].set(def_id);
        else
            m_gen[blk_idx].reset(def_id);
    }
}

void SimpleConstantPropagation::kill(int blk_idx, Tac* tac, Operand* oper)
{
    for(auto& def_id: oper->def_id)
    {
        assert(m_definitions[def_id].id == def_id);
        assert(m_definitions[def_id].dest == oper);
        //kill所有除该定值以外的所有定值
        if(m_definitions[def_id].tac != tac)
            continue;
        m_kill[blk_idx].set(def_id);
    }
}

void SimpleConstantPropagation::print_reaching_definition()
{
    std::cout << "---Reaching Definition---" << std::endl;
    for(auto& def: m_definitions)
    {
        std::cout << "d" << def.id << ": ";
        def.dest->dump();
        std::cout << " at " << def.dest;
        def.tac->dump();
    }
    for(int i = 0; i < m_gen.size(); i++)
    {
        // std::cout << "block[" << i << "]" << std::endl;
        // std::cout << "  gen  = " << m_gen[i].get_string() << std::endl;
        // std::cout << "  kill = " << m_kill[i].get_string() << std::endl;
        // std::cout << "  in   = " << m_in[i].get_string() << std::endl;
        // std::cout << "  out  = " << m_out[i].get_string() << std::endl;
    }
    std::cout << "-------------------------" << std::endl;
}