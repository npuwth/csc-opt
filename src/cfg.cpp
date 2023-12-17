#include "cfg.h"

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

void CFGBlock::add_tac(Tac* tac)
{
    m_tac_list.push_back(tac);
}

void CFGBlock::add_tac_to_front(Tac* tac)
{
    m_tac_list.push_front(tac);
}

CFGProcedure* CFGProcedure::create_procedure()
{
    auto entry = new CFGBlock(-1);
    auto exit = new CFGBlock(-2);
    return new CFGProcedure(0, entry, exit);
}

CFGProgram* CFGManager::gen_from_program(Program& prog)
{
    CFGProgram* cfg_prog = new CFGProgram;
    std::unordered_map<int, CFGBlock*> id_to_block;
    //TODO:遍历Program生存控制流图
    for(auto& scope: prog.Scopes)
    {
        //每个Scope对应一个CFGProcedure，也即一个函数过程
        CFGProcedure* cfg_proc = CFGProcedure::create_procedure();
        int first_tacid = scope->Tacs.front()->getTacID();
        int tacid = 0;
        //第一遍遍历tac，确定基本块开头并创建基本块
        for(auto& tac: scope->Tacs)
        {
            if(tac->getTacID() == first_tacid)
            {
                tacid = tac->getTacID();
                //第一条指令，创建基本块
                if(id_to_block[tacid] == nullptr)
                {
                    CFGBlock* new_block = new CFGBlock(tacid);
                    id_to_block[tacid] = new_block;
                }
            }
            else if(tac->getOpcode() == Type::BR || tac->getOpcode() == Type::BLBC || tac->getOpcode() == Type::BLBS)
            {
                tacid = ((Label*)(tac->getSrc1()))->instr_pos;
                //跳转指令的目的，创建基本块
                if(id_to_block[tacid] == nullptr)
                {
                    CFGBlock* new_block = new CFGBlock(tacid);
                    id_to_block[tacid] = new_block;
                }
                tacid = tac->getTacID() + 1;
                //跳转指令的后继，创建基本块
                if(id_to_block[tacid] == nullptr)
                {
                    CFGBlock* new_block = new CFGBlock(tacid);
                    id_to_block[tacid] = new_block;
                }
            }
        }
        if(id_to_block[first_tacid] == nullptr)
            cfg_error();
        cfg_proc->get_entry()->set_edge(0, id_to_block[first_tacid]);
        CFGBlock* current_block = id_to_block[first_tacid];
        //第二遍遍历tac，向基本块中添加指令并连接基本块
        for(auto& tac: scope->Tacs)
        {
            int succ_tacid = 0;
            tacid = tac->getTacID();
            if(id_to_block[tacid] != nullptr)   //切换当前基本块
                current_block = id_to_block[tacid];
            if(current_block == nullptr)
                std::cout << "[Error] block pointer is null!" << std::endl;

            if(tac->getOpcode() == Type::BR)                                      //无条件跳转
            {
                succ_tacid = ((Label*)(tac->getSrc1()))->instr_pos;
                if(id_to_block[succ_tacid] == nullptr)
                    cfg_error();
                current_block->set_edge(0, id_to_block[succ_tacid]);
            }
            else if(tac->getOpcode() == Type::BLBC || tac->getOpcode() == Type::BLBS)   //条件跳转
            {
                // False分支
                succ_tacid = tac->getTacID() + 1;
                if(id_to_block[succ_tacid] == nullptr)
                    cfg_error();
                current_block->set_edge(0, id_to_block[succ_tacid]);
                // True分支
                succ_tacid = ((Label*)(tac->getSrc1()))->instr_pos;
                if(id_to_block[succ_tacid] == nullptr)
                    cfg_error();
                current_block->set_edge(1, id_to_block[succ_tacid]);
            }
            else if(tac->getOpcode() == Type::ENTER)                              //过程进入
            {
                cfg_proc->set_index(tacid);
            }
            else if(tac->getOpcode() == Type::RET)                                //过程返回
            {
                current_block->set_edge(0, cfg_proc->get_exit());
            }
            current_block->add_tac(tac);
        }
        cfg_prog->push_back(cfg_proc);
    }
    return cfg_prog;
}

void CFGManager::dump_cfg_block(CFGBlock* block,std::ostream& os)
{
    os << "Basic Blocks: " << block->get_index() << std::endl;
    os << block->get_index() << " -> " << block->get_succ(0)->get_index();
    if(block->out_degree() == 2)
        os << " " << block->get_succ(1)->get_index();
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

void CFGManager::dump_cfg_procedure(CFGProcedure* proc,std::ostream& os)
{
    os << "Function: " << proc->get_index() << std::endl;
    dump_cfg(proc->get_entry(), os);
}

void CFGManager::dump_cfg_program(CFGProgram& prog,std::ostream& os)
{
    for(auto& proc: prog)
        dump_cfg_procedure(proc, os);

}

void CFGManager::cfg_error()
{
    std::cout << "[Error] block pointer is null!" << std::endl;
    exit(255);
}