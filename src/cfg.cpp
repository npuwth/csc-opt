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
        if(scope->ScopeID == 0)
            continue;
        //每个Scope对应一个CFGProcedure，也即一个函数过程
        CFGProcedure* cfg_proc = CFGProcedure::create_procedure();
        if(scope->MainScope)
            cfg_proc->set_main();
        if(scope->Tac_head == nullptr) printf("Error: Empty Scope!\n");
        int first_tacid = scope->Tac_head->getTacID();
        int tacid = 0;
        //第一遍遍历tac，确定基本块开头并创建基本块
        Tac *tac = scope->Tac_head;
        for(;tac != nullptr; tac = tac->getSucTac())
        {
            switch(tac->getOpcode())
            {
                case Type::ENTER:
                    tacid = tac->getTacID();
                    assert(first_tacid == tacid);
                    //第一条指令，创建基本块
                    if(id_to_block[tacid] == nullptr)
                    {
                        CFGBlock* new_block = new CFGBlock(tacid);
                        id_to_block[tacid] = new_block;
                    }
                    break;
                case Type::BR: case Type::CALL: case Type::BLBC: case Type::BLBS:
                    if(tac->getOpcode() == Type::BR || tac->getOpcode() == Type::CALL)
                        tacid = ((Label*)(tac->getSrc0()))->instr_pos;
                    else
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
                    break;
                default:break;
            }
        }
        if(id_to_block[first_tacid] == nullptr)
            cfg_error();
        cfg_proc->get_entry()->set_edge(0, id_to_block[first_tacid]);
        CFGBlock* current_block = id_to_block[first_tacid];
        //第二遍遍历tac，向基本块中添加指令并连接基本块
        tac = scope->Tac_head;
        for(;tac != nullptr; tac = tac->getSucTac())
        {
            int succ_tacid = 0;
            tacid = tac->getTacID();
            if(id_to_block[tacid] != nullptr)   //切换当前基本块
            {
                Tac* last_tac = current_block->get_tac_list().back();
                if(last_tac != nullptr)
                {
                    switch(last_tac->getOpcode())
                    {
                        case Type::BR: case Type::CALL: case Type::BLBC: case Type::BLBS: case Type::RET: break;
                        default:
                            current_block->set_edge(0, id_to_block[tacid]);
                            break;
                    }
                }
                current_block = id_to_block[tacid];
                cfg_proc->add_block(current_block);
            }
            if(current_block == nullptr)
                    cfg_error();
            switch(tac->getOpcode())
            {
                case Type::BR://无条件跳转
                    succ_tacid = ((Label*)(tac->getSrc0()))->instr_pos;
                    if(id_to_block[succ_tacid] == nullptr)
                        cfg_error();
                    current_block->set_edge(0, id_to_block[succ_tacid]);
                    break;
                case Type::BLBC: case Type::BLBS://条件跳转
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
                    break;
                case Type::CALL://过程调用
                    succ_tacid = tac->getTacID() + 1;
                    if(id_to_block[succ_tacid] == nullptr)
                        cfg_error();
                    current_block->set_edge(0, id_to_block[succ_tacid]);
                    break;
                case Type::ENTER://过程进入
                    cfg_proc->set_id(tacid);
                    break;
                case Type::RET://过程返回
                    current_block->set_edge(0, cfg_proc->get_exit());
                    break;
                default:break;
            }
            current_block->add_tac(tac);
        }
        cfg_prog->push_back(cfg_proc);
    }
    return cfg_prog;
}

void CFGManager::dump_cfg_block(CFGBlock* block,std::ostream& os, bool show_all)
{
    if(block->get_id() < 0)
        return; //不输出entry和exit
    if(show_all)
    {
        os << "Basic Block: " << block->get_id() << std::endl;
        os << "---TACs:" << std::endl;
        for(auto& tac: block->get_tac_list())
            tac->dump();
        os << "---Edges:" << std::endl << "\t";
    }
    os << block->get_id() << " -> ";
    if(block->get_succ(0)->get_id() >= 0)
        os << block->get_succ(0)->get_id();
    if(block->out_degree() == 2 && block->get_succ(1)->get_id() >= 0)
        os << " " << block->get_succ(1)->get_id();
    os << std::endl;
    if(show_all)
        os << std::endl;
}

void CFGManager::dump_cfg(CFGBlock* entry,std::ostream& os, bool show_all)
{
    // bfs序输出基本块
    std::set<CFGBlock*> is_printed;
    std::queue<CFGBlock*> q;
    q.push(entry);
    while(!q.empty()){
        CFGBlock* now = q.front();
        q.pop();
        dump_cfg_block(now, os, show_all);
        for (int k = 1; k >= 0; --k) {
            if (now->get_succ(k) != nullptr && is_printed.find(now->get_succ(k)) == is_printed.end()) {
                q.push(now->get_succ(k));
                is_printed.insert(now->get_succ(k));
            }
        }
    }
}

void CFGManager::dump_cfg_procedure(CFGProcedure* proc,std::ostream& os, bool show_all)
{
    os << "Function: " << proc->get_id();
    if(show_all && proc->is_main())
        os << " main" << std::endl;
    os << std::endl;
    if(show_all)
        dump_cfg(proc->get_entry(), os, show_all);
    else
    {
        os << "Basic Blocks:";
        for(auto& block: proc->get_blocks())
            std::cout << " " << block->get_id();
        os << std::endl;
        os << "CFG:" << std::endl;
        for(auto& block: proc->get_blocks())
            dump_cfg_block(block, os);
    }
}

void CFGManager::dump_cfg_program(CFGProgram& prog,std::ostream& os, bool show_all)
{
    for(auto& proc: prog)
        dump_cfg_procedure(proc, os, show_all);

}

void CFGManager::cfg_error()
{
    std::cout << "[Error] block pointer is null!" << std::endl;
    exit(255);
}