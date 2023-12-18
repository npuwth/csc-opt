#ifndef __CFG_H__
#define __CFG_H__
#include "tac.h"
#include "list"
#include <vector>
#include <array>
#include <set>
#include <queue>
#include <iostream>
#include <cassert>

using TacList=std::list<Tac*>;


class CFGBlock //CFG基本块
{
private:
    int m_index{0};                         //基本块编号
    std::multiset<CFGBlock*> m_anti_edge{}; //反向边
    std::array<CFGBlock*, 2> m_edge;        //正向边
    TacList m_tac_list;                     //指令列表
    void erase_from_anti_edge(CFGBlock* target);     //从反向边表中删除指定边
public:
    CFGBlock(int i_index) { m_edge[0] = m_edge[1] = nullptr; m_tac_list = TacList(); m_index = i_index; }

    void add_tac(Tac* tac);
    void add_tac_to_front(Tac* tac);
    void set_edge(int index,CFGBlock* to);   //设置边的函数，同时也会在目标节点添加反向边
    void delete_edge(int index);            //从邻接表中删除某条边,同时也会将目标节点中的反向边删除
    //get函数
    int get_index() { return m_index; }
    TacList& get_tac_list() { return m_tac_list; }
    CFGBlock* get_succ(int index=0){return m_edge[index];}                         //获取后继节点
    std::multiset<CFGBlock*>& get_pred(){ return m_anti_edge; }                   //获取前驱节点列表
    int in_degree(){ return m_anti_edge.size(); }                                 //入度
    int out_degree(){return (m_edge[0]!=nullptr?1:0)+(m_edge[1]!=nullptr?1:0);}   //出度
    bool is_entry(){ return m_anti_edge.empty(); }                                //是否为入口
    bool is_exit(){return m_edge[0]==nullptr&&m_edge[1]==nullptr;}                //是否为出口
};

class CFGProcedure //CFG过程，对应一个Scope或一个函数
{
private:
    int m_index{0};                              //过程编号
    CFGBlock* m_entry;                           //表示虚拟的入口块，内部为空
    CFGBlock* m_exit;                            //表示虚拟的出口块，内部为空
    std::list<CFGBlock*> m_blocks;                //记录所有基本块
    bool m_main{false};                          //是否为main函数
public:
    CFGProcedure(int i_index, CFGBlock* i_entry=nullptr, CFGBlock* i_exit=nullptr):m_entry(i_entry),m_exit(i_exit),m_index(i_index){m_main = false;}
    ~CFGProcedure(){}
    static CFGProcedure* create_procedure();
    //set
    void set_index(int i_index) {m_index = i_index;}
    void set_entry(CFGBlock* i_entry) {m_entry = i_entry;}
    void set_exit(CFGBlock* i_exit) {m_exit = i_exit;}
    void set_main() {m_main = true;}
    void add_block(CFGBlock* block) {m_blocks.push_back(block);}
    //get
    int get_index() {return m_index;}
    CFGBlock* get_entry() {return m_entry;}
    CFGBlock* get_exit() {return m_exit;}
    bool is_main() {return m_main;}
    std::list<CFGBlock*>& get_blocks() {return m_blocks;}
};

using CFGProgram=std::list<CFGProcedure*>;

class CFGManager
{
private:
    static void cfg_error();
public:
    static CFGProgram* gen_from_program(Program& prog);             //从Program生成CFGProgram
    static void dump_cfg_block(CFGBlock* block,std::ostream& os, bool simple = true);
    static void dump_cfg(CFGBlock* entry,std::ostream& os);
    static void dump_cfg_procedure(CFGProcedure* proc,std::ostream& os, bool show_all = false);
    static void dump_cfg_program(CFGProgram& prog,std::ostream& os=std::cout);
};


#endif