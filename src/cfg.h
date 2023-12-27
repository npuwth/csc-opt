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

using TacList = std::list<Tac*>;

class CFGBlock { //CFG基本块
private:
    int m_id = 0;                           //基本块编号（仅用于输出）
                                            //Entry为-1，Exit为-2
    int m_index = -1;                       //基本块索引（Entry为0，Exit为1）
    std::multiset<CFGBlock*> m_anti_edge;   //反向边，入边（数目不限）
    std::array<CFGBlock*, 2> m_edge;        //正向边，出边（至多两个）
    TacList m_tac_list;                     //指令列表
    // std::set<int> m_dominator;              //存放该基本块的支配者的索引
    CFGBlock* m_idominator = nullptr;       //直接支配节点
    std::set<CFGBlock*> m_domfront;         //支配边界

    void erase_from_anti_edge(CFGBlock* target); //从反向边表中删除指定边
public:
    CFGBlock(int i_id) { 
        m_edge[0] = m_edge[1] = nullptr; 
        m_tac_list = TacList(); 
        m_id = i_id; 
    }

    void add_tac(Tac* tac);
    void add_tac_to_front(Tac* tac);
    //set
    void set_edge(int index, CFGBlock* to);   //设置边的函数，同时也会在目标节点添加反向边
    void delete_edge(int index);             //从邻接表中删除某条边,同时也会将目标节点中的反向边删除
    void set_id(int id) { m_id = id;}
    void set_index(int index) { m_index = index;}
    // void add_dominator(int index) { m_dominator.insert(index); }
    void set_idominator(CFGBlock* idom) { m_idominator = idom; }
    void add_domfront(CFGBlock* runner) { m_domfront.insert(runner); }
    //get
    int get_index() { return m_index; }
    int get_id() { return m_id; }
    TacList& get_tac_list() { return m_tac_list; }
    CFGBlock* get_succ(int index){ return m_edge[index];}                          //获取后继节点
    std::array<CFGBlock*, 2> get_succ(){ return m_edge;}                           //获取后继节点列表
    std::multiset<CFGBlock*>& get_pred(){ return m_anti_edge; }                    //获取前驱节点列表
    int in_degree(){ return m_anti_edge.size(); }                                  //入度
    int out_degree(){ return (m_edge[0]!=nullptr?1:0)+(m_edge[1]!=nullptr?1:0);}   //出度
    bool is_entry(){ return m_anti_edge.empty(); }                                 //是否为入口
    bool is_exit(){ return m_edge[0]==nullptr&&m_edge[1]==nullptr;}                //是否为出口
    // std::set<int> get_dominator() { return m_dominator; }
    CFGBlock* get_idominator() { return m_idominator; }
    std::set<CFGBlock*>& get_domfront() { return m_domfront; }
};

class CFGProcedure { //CFG过程，对应一个Scope
private:
    int m_id = 0;                     //过程编号
    CFGBlock* m_entry;                //表示虚拟的入口块，内部为空
    CFGBlock* m_exit;                 //表示虚拟的出口块，内部为空
    std::vector<CFGBlock*> m_blocks;    //所有基本块, 包含entry和exit
    bool m_main = false;              //是否为main函数
    std::unordered_map<std::string, Operand*> m_sym; //Scope中的symbol table
public:
    CFGProcedure(int i_id, Scope* i_scope, CFGBlock* i_entry=nullptr, CFGBlock* i_exit=nullptr) {
        m_id = i_id;
        m_entry = i_entry;
        m_exit = i_exit;
        m_main = i_scope->MainScope;
        m_sym = i_scope->sym;
    }
    ~CFGProcedure(){}

    static CFGProcedure* create_procedure(Scope* scope);
    //set
    void set_id(int i_id) { m_id = i_id;}
    void set_entry(CFGBlock* i_entry) { m_entry = i_entry;}
    void set_exit(CFGBlock* i_exit) { m_exit = i_exit;}
    void set_main() { m_main = true;}
    void add_block(CFGBlock* block) { block->set_index(m_blocks.size()); m_blocks.push_back(block);}
    //get
    int get_id() {return m_id;}
    CFGBlock* get_entry() { return m_entry;}
    CFGBlock* get_exit() { return m_exit;}
    bool is_main() { return m_main;}
    std::vector<CFGBlock*>& get_blocks() { return m_blocks;}
    std::unordered_map<std::string, Operand*>& get_sym() { return m_sym;}
};

class CFGProgram {
public:
    std::list<CFGProcedure*> pl;
    dt_long maxTacID;
    CFGProgram(){}
    ~CFGProgram(){}
};

class CFGManager {
private:
    static void cfg_error();
public:
    static CFGProgram* gen_from_program(Program& prog);             //从Program生成CFGProgram
    static void dump_cfg_block(CFGBlock* block,std::ostream& os, bool show_all = false);
    static void dump_cfg(CFGBlock* entry,std::ostream& os, bool show_all = false);
    static void dump_cfg_procedure(CFGProcedure* proc,std::ostream& os, bool show_all = false);
    static void dump_cfg_program(CFGProgram& prog,std::ostream& os=std::cout, bool show_all = false);
};


#endif