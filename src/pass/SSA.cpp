#include "SSA.h"
#include <algorithm>

void ConvertSSA::run() {
    std::cout << "INFO: Running SSA Pass..." << std::endl;
    cmaxTacID = m_cfg->maxTacID + 1;
    for(auto& proc: m_cfg->pl) {
        //step1: 计算支配边界
        compute_reversePostOrder(proc);
        compute_immediate_dominator(proc);
        compute_dominance_frontiers(proc);

        //step2: 插入PHI函数
        insert_phi_functions(proc);

        //step3: 重命名变量
        visited.resize(proc->get_blocks().size() , false);
        varMap.clear();
        varCount.clear();
        varStack.clear();
        varID = 0;
        for(auto& sym: proc->get_sym()) {
            if(sym.second->getType() == OperandType::VAR) {
                varMap[(Variable*)sym.second] = varID;
                varCount[varID] = 0;
                // varStack[varID].push((Variable*)sym.second);
                varID++;
            }
        }
        rename_variables(proc->get_entry());
    }
}

void ConvertSSA::compute_reversePostOrder(CFGProcedure* proc) {
    //debug
    // printf("blocks:\n");
    // for(auto& blk: proc->get_blocks()) {
    //     printf("id: %d\tidx: %d\n", blk->get_id(), blk->get_index());
    // }
    int pid = 0;
    postOrderID.resize(proc->get_blocks().size()); //idx -> postOrder
    std::stack<CFGBlock*> s;
    visited.resize(proc->get_blocks().size() , false);
    s.push(proc->get_entry());
    visited[proc->get_entry()->get_index()] = true;
    while(!s.empty()) {
        CFGBlock* t = s.top();
        bool tail = true;
        for(auto& succ: t->get_succ()) {
            if(succ == nullptr) continue;
            if(!visited[succ->get_index()]) {
                tail = false;
                s.push(succ);
                //debug
                // printf("%d\n", succ->get_index());
                visited[succ->get_index()] = true;
                break;
            }
        }
        if(tail) {
            s.pop();
            postOrderID[t->get_index()] = pid++;
        }
    }
    reversePostOrder.resize(proc->get_blocks().size());
    int maxPostOrderID = pid - 1;
    for(size_t i = 0; i < reversePostOrder.size(); i++) {
        reversePostOrder[maxPostOrderID - postOrderID[i]] = i;
    }
}

CFGBlock* ConvertSSA::intersect(CFGBlock* x, CFGBlock* y) {
    while(x != y) {
        while(postOrderID[x->get_index()] < postOrderID[y->get_index()]) {
            x = x->get_idominator();
        }
        while(postOrderID[y->get_index()] < postOrderID[x->get_index()]) {
            y = y->get_idominator();
        }
    }
    return x;
}

void ConvertSSA::compute_immediate_dominator(CFGProcedure* proc) {
    // 计算dominators
    // for(auto& blk: proc->get_blocks()) {
    //     blk->add_dominator(blk->get_index());
    // }
    // while(true) {
    //     bool changed = false;
    //     for(auto& blk: proc->get_blocks()) {
    //         int old_size = blk->get_dominator().size();
    //         std::set<int> new_dom; bool tag = 0;
    //         std::vector<int> temp;
    //         for(auto& pred: blk->get_pred()) {
    //             if(tag == 0) {
    //                 tag = 1;
    //                 new_dom = pred->get_dominator();
    //             } else {
    //                 temp.clear();
    //                 std::set_intersection(new_dom.begin(), new_dom.end(), 
    //                     pred->get_dominator().begin(), pred->get_dominator().end(),
    //                     temp.begin());
    //                 for(auto& dom: temp) {
    //                     new_dom.insert(std::move(dom));
    //                 }
    //             }
    //         }
    //         for(auto& dom: new_dom) {
    //             blk->add_dominator(dom);
    //         }
    //         int new_size = blk->get_dominator().size();
    //         if(new_size != old_size) {
    //             changed = true;
    //         }
    //     }
    //     if(!changed) break;
    // }
    // //计算immediate dominator
    // for(auto& blk: proc->get_blocks()) {
    //     if(blk->get_dominator().size() == 1) { //只包含自己，没有IDOM
    //         ;
    //     } else {
    //         int index = 0;
    //     }
    // }

    proc->get_entry()->set_idominator(proc->get_entry());
    while(true) {
        bool changed = false;
        for(size_t i = 0; i < reversePostOrder.size(); i++) { //reverse postOrder
            CFGBlock* blk = proc->get_blocks()[i];
            if(blk->get_index() == 0) continue; //跳过entry block
            if(blk->get_pred().size() == 0) {
                printf("Error: Exist Unreachable Block!\n");
                return;
            }
            auto it = blk->get_pred().begin();
            CFGBlock* idom = *it;
            for(; (idom->get_idominator() == nullptr || idom == blk) && it != blk->get_pred().end(); it++) {
                idom = *it;
            } //尝试寻找一个已经有idom的前驱
            for(; it != blk->get_pred().end(); it++) {
                if((*it)->get_idominator() != nullptr && *it != blk) {
                    idom = intersect(idom, *it);
                }
            }
            if(blk->get_idominator() != idom) {
                changed = true;
                //debug
                // printf("changed\n");
                blk->set_idominator(idom);
            }
        }
        if(!changed) break;
    }
    //debug
    // printf("Idom:\n");
    // for(auto& blk: proc->get_blocks()) {
    //     printf("id: %d\tidom: %d\n", blk->get_id(), blk->get_idominator()->get_id());
    // }
}

void ConvertSSA::compute_dominance_frontiers(CFGProcedure* proc) {
    for(auto& blk: proc->get_blocks()) {
        if(blk->get_index() == 1) continue; //跳过exit block
        if(blk->get_pred().size() >= 2) {
            CFGBlock* idom = blk->get_idominator();
            for(auto& pred: blk->get_pred()) {
                CFGBlock* runner = pred;
                while(runner != idom) {
                    blk->add_domfront(runner);
                    runner = runner->get_idominator();
                }
            }
        }
    }
}

void ConvertSSA::insert_phi_functions(CFGProcedure* proc) {

}

Variable* ConvertSSA::gen_name(Variable* var) {
    int vid = varMap[var];
    int i = varCount[vid];
    varCount[vid]++;
    std::string old_name = var->getName().substr(0, var->getName().find("$"));
    std::string new_name =  old_name + "$" + std::to_string(i);
    Variable* op = new Variable(new_name, var->getOffset());
    varStack[vid].push(op);
    return op;
}

void ConvertSSA::rename_variables(CFGBlock* blk) {
    if(visited[blk->get_index()]) return;
    visited[blk->get_index()] = true;
    TacList tl = blk->get_tac_list();
    auto it = tl.begin();
    for(; it != tl.end(); ) {
        Tac* tac = *it;
        if(tac->getOpcode() != Type::PHI) break;
        Variable* src = (Variable*)tac->getSrc0();
        Operand* op = gen_name(src);
        Tac* mov = new Tac(cmaxTacID++, Type::MOVE, "move", tac->getDest(), op); //添加move指令
        it++;
        tl.insert(it, mov); //在it位置插入mov，并指向下一个tac
    }
    for(; it != tl.end(); it++) {
        Tac* tac = *it;
        Operand *src0, *src1;
        src0 = tac->getSrc0(); //RHS
        if(src0->getType() == OperandType::VAR) { //只需要处理Variable
            tac->rebindSrc0(varStack[varMap[(Variable*)src0]].top());
        }
        src1 = tac->getSrc1(); //LHS(MOVE) or RHS
        if(src1->getType() == OperandType::VAR) {
            // tac->rebindSrc1(varStack[varMap[(Variable*)src1]].top());
            if(tac->getOpcode() == Type::MOVE) {
                Operand* op = gen_name((Variable*)src1);
                tac->rebindSrc1(op);
            } else {
                tac->rebindSrc1(varStack[varMap[(Variable*)src1]].top());
            }
        }
        // Operand* dest = tac->getDest();
        // if(dest->getType() == OperandType::VAR) {
        // }
    }
    for(auto& succ: blk->get_succ()) {

    }
}
