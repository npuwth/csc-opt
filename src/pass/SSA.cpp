#include "SSA.h"
#include <algorithm>

void ConvertSSA::run() {
    std::cout << "INFO: Running SSA Pass..." << std::endl;
}

void ConvertSSA::compute_immediate_dominator() {
    //计算dominators
    for(auto& proc: *m_cfg) {
        for(auto& blk: proc->get_blocks()) {
            blk->add_dominator(blk->get_index());
        }

        while(true) {
            bool changed = false;
            for(auto& blk: proc->get_blocks()) {
                int old_size = blk->get_dominator().size();
                std::set<int> new_dom; bool tag = 0;
                std::vector<int> temp;
                for(auto& pred: blk->get_pred()) {
                    if(tag == 0) {
                        tag = 1;
                        new_dom = pred->get_dominator();
                    } else {
                        temp.clear();
                        std::set_intersection(new_dom.begin(), new_dom.end(), 
                            pred->get_dominator().begin(), pred->get_dominator().end(),
                            temp.begin());
                        for(auto& dom: temp) {
                            new_dom.insert(std::move(dom));
                        }
                    }
                }
                for(auto& dom: new_dom) {
                    blk->add_dominator(dom);
                }

                int new_size = blk->get_dominator().size();
                if(new_size != old_size) {
                    changed = true;
                }
            }
            if(!changed) break;
        }
    }
    //计算immediate dominator
    for(auto& proc: *m_cfg) {
        for(auto& blk: proc->get_blocks()) {

        }
    }
}

void ConvertSSA::compute_dominance_frontiers() {

}

void ConvertSSA::insert_phi_functions() {

}

void ConvertSSA::rename_variables() {

}
