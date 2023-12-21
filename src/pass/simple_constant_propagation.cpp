#include "simple_constant_propagation.h"

void SimpleConstantPropagation::run() {
    std::cout << "running SCP pass..." << std::endl;
    for(auto& proc: *m_cfg)
    {
        initial_definitions(proc);
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
                    std::cout << "d" << def_id << ": ";
                    tac->getSrc1()->dump();
                    std::cout << " at " << tac->getSrc1();
                    tac->dump();
                    def_id++;
                //对dest定值
                default:
                    m_definitions.push_back(SCP::Definition(def_id, tac->getDest(), tac));
                    tac->getDest()->def_id.push_back(def_id);
                    std::cout << "d" << def_id << ": ";
                    tac->getDest()->dump();
                    std::cout << " at " << tac->getDest();
                    tac->dump();
                    def_id++;
                    break;
            }
        }
    }
    int blk_num = proc->get_blocks().size() + 2;    //再加上ENTRY和EXIT这两个
    int def_num = m_definitions.size();
    std::cout << "blk num: " << blk_num << std::endl;
    std::cout << "def num: " << def_num << std::endl;
    for(int i = 0; i < blk_num; i++)
    {
        m_gen.push_back(BitMap(def_num));
        m_kill.push_back(BitMap(def_num));
        m_in.push_back(BitMap(def_num));
        m_out.push_back(BitMap(def_num));
    }
}
void SimpleConstantPropagation::compute_gen_and_kill()
{

}
void SimpleConstantPropagation::compute_in_and_out()
{

}
