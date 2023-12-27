#include "SSA.h"
#include <algorithm>

void ConvertSSA::run() {
    std::cout << "INFO: Running SSA Pass..." << std::endl;
    for(auto& proc: *m_cfg) {
        compute_reversePostOrder(proc);
        compute_immediate_dominator(proc);
        compute_dominance_frontiers(proc);
        insert_phi_functions(proc);
        rename_variables(proc);
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
    std::vector<bool> visited(proc->get_blocks().size() , false);
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
    printf("Idom:\n");
    for(auto& blk: proc->get_blocks()) {
        printf("id: %d\tidom: %d\n", blk->get_id(), blk->get_idominator()->get_id());
    }
}

void ConvertSSA::compute_dominance_frontiers(CFGProcedure* proc) {

}

void ConvertSSA::insert_phi_functions(CFGProcedure* proc) {

}

void ConvertSSA::rename_variables(CFGProcedure* proc) {

}
