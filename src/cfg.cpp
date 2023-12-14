#include "cfg.h"
int CFGBlock::s_index = 0;

void CFGBlock::erase_from_anti_edge(CFGBlock* target)
{
    if (auto iter = m_anti_edge.find(target); iter != m_anti_edge.end()) {
        m_anti_edge.erase(iter);
    }
}

void CFGBlock::set_edge(int index,CFGBlock* to)
{
    m_edge[index] = to;
    if(to != nullptr)
        to->m_anti_edge.insert(this);
}

void CFGBlock::delete_edge(int index)
{
    CFGBlock* target = m_edge[index];
    if(target != nullptr)
        target->erase_from_anti_edge(this);
    m_edge[index] = nullptr;
}

void CFGBlock::add_tac(Tac& tac)
{
    m_tac_list.push_back(tac);
}

void CFGBlock::add_tac_to_front(Tac& tac)
{
    m_tac_list.push_front(tac);
}

CFGProcedure CFGProcedure::create_procedure()
{
    auto entry = new CFGBlock();
    auto exit = new CFGBlock();
    return CFGProcedure(entry, exit);
}

CFGProgram* gen_from_program(Program& prog)
{
    CFGProgram* cfg_prog = new CFGProgram;
    //TODO:遍历Program生存控制流图
    return cfg_prog;
}

void CFGManager::dump_cfg_block(CFGBlock* block,std::ostream& os)
{
    os << "....Block B" << block->get_index();
    if (block->is_entry()) os << " [Entry]";
    if (block->is_exit()) os << " [Exit]";
    os << " [In " << block->in_degree() << " Out " << block->out_degree() << "]";
    os << std::endl;

    // 打印tac,暂未支持std::ostream
    for(auto& tac: block->get_tac_list())
        tac.dump();
    
    if(block->out_degree() == 1){
        os << "....Block End [Uncond] B"<< block->get_succ(0)->get_index() << std::endl;
    }else if(block->out_degree() == 2){
        os << "....Block End [True] B"<< block->get_succ(1)->get_index();
        os << " [False] B"<< block->get_succ(0)->get_index() << std::endl;
    }
    os << std::endl;

}

void CFGManager::dump_cfg(CFGBlock* entry,std::ostream& os)
{
    // bfs序输出基本块
    std::set<CFGBlock*> is_printed;
    std::queue<CFGBlock*> q;
    q.push(entry);
    while(!q.empty()){
        CFGBlock* now = q.front();
        q.pop();
        dump_cfg_block(now,os);
        for (int k = 1; k >= 0; --k) {
            if (now->get_succ(k) != nullptr && is_printed.find(now->get_succ(k)) == is_printed.end()) {
                q.push(now->get_succ(k));
                is_printed.insert(now->get_succ(k));
            }
        }
    }
}

void CFGManager::dump_cfg_procedure(CFGProcedure& proc,std::ostream& os)
{
    dump_cfg(proc.get_entry(), os);
}

void CFGManager::dump_cfg_program(CFGProgram& prog,std::ostream& os)
{
    for(auto& proc: prog)
        dump_cfg_procedure(proc, os);

}