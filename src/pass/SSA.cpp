#include "SSA.h"
#include <algorithm>

void ConvertSSA::run() {
    if(PASS_DEBUG) {
        std::cout << "INFO: Running SSA Pass..." << std::endl;
    }
    cmaxTacID = m_cfg->maxTacID + 1;
    for(auto& proc: m_cfg->pl) {
        curProc = proc;
        //step1: 计算支配边界
        compute_reversePostOrder(proc);
        compute_immediate_dominator(proc);
        compute_dominance_frontiers(proc);

        //step2: 插入PHI函数
        varMap.clear();
        varCount.clear();
        varStack.clear();
        varID = 0;
        //debug
        // printf("Variables in Symbol Table:\n");
        // for(auto& sym: proc->get_sym()) {
        //     if(sym.second->getType() == OperandType::VAR)
        //     std::cout << "  " << ((Variable*)sym.second)->getName() << std::endl;
        // }
        std::unordered_map<std::string, Operand*> syms(proc->get_sym());
        for(auto& sym: syms) {
            if(sym.second->getType() == OperandType::VAR) { //只需要对Variable做SSA
                varMap[(Variable*)sym.second] = varID;
                //debug
                // printf("Insert in VarMap: ");
                // std::cout << ((Variable*)sym.second)->getName() << std::endl;
                varCount.push_back(0);
                std::stack<Variable*> s;
                // s.push((Variable*)sym.second);
                varStack.push_back(s);
                gen_name((Variable*)sym.second);
                varID++;
            }
        }
        insert_phi_functions(proc);

        //step3: 重命名变量
        visited.clear();
        visited.resize(proc->get_blocks().size(), false);
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
    postOrderID.clear();
    postOrderID.resize(proc->get_blocks().size()); //idx -> postOrder
    std::stack<CFGBlock*> s;
    visited.clear();
    visited.resize(proc->get_blocks().size(), false);
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
    reversePostOrder.clear();
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
    // debug
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
                // if(PASS_DEBUG) {
                //     printf("%d != %d\n", runner->get_index(), idom->get_index());
                // }
                while(runner != idom) {
                    runner->add_domfront(blk);
                    runner = runner->get_idominator();
                }
            }
        }
    }
    // debug
    // printf("Domfront:\n");
    // for(auto& blk: proc->get_blocks()) {
    //     printf("id: %d\tdomfront size: %ld\n", blk->get_id(), blk->get_domfront().size());
    // }
}

void ConvertSSA::insert_phi_functions(CFGProcedure* proc) {
    if(PASS_DEBUG) {
        printf("VarNum: %ld\n", varMap.size());
    }
    for(auto& var: varMap) {
        hasAlready.clear();
        everOnWorkList.clear();
        while(!workList.empty()) workList.pop();
        for(auto& blk: proc->get_blocks()) {
            if(blk->get_index() == 0 || blk->get_index() == 1) continue;
            bool tag = 0;
            for(auto& tac: blk->get_tac_list()) {
                if(tac->getOpcode() == Type::MOVE) //LHS
                    if(tac->getSrc1()->getType() == OperandType::VAR && (Variable*)(tac->getSrc1()) == var.first) {
                        tag = 1;
                        break;
                    }
            }
            if(tag) {
                if(PASS_DEBUG) {
                    printf("INFO: Add To WorkList.\n");
                }
                everOnWorkList.insert(blk);
                workList.push(blk);
            }
        }
        while(!workList.empty()) {
            CFGBlock* t = workList.front(); workList.pop();
            if(PASS_DEBUG) {
                printf("Using Block %d\n", t->get_id());
                printf("domfront size: %ld\n", t->get_domfront().size());
            }
            for(auto& succ: t->get_domfront()) {
                if(hasAlready.find(succ) == hasAlready.end()) {
                    hasAlready.insert(succ);
                    Tac* tac = new Tac(cmaxTacID++, Type::PHI, std::string("phi"), var.first);
                    curProc->get_sym().insert({"(" + std::to_string(tac->getTacID()) + ")", tac->getDest()});
                    // if(PASS_DEBUG) {
                    //     printf("INFO: Insert PHI Function.\n"); //debug
                    // }
                    auto& tl = succ->get_tac_list();
                    bool tag = 0;
                    auto it = tl.begin();
                    while((*it)->getOpcode() == Type::PHI) {
                        tag = 1;
                        it++;
                    }
                    tl.insert(it, tac); //插在第一个非PHI之前
                    Tac* suc_tac = *it;
                    Tac* pre_tac = (*it)->getPreTac();
                    pre_tac->setSucTac(tac);
                    suc_tac->setPreTac(tac);
                    tac->setPreTac(pre_tac);
                    tac->setSucTac(suc_tac);
                    if(!tag) { //更新前驱的br
                        for(auto& pre: succ->get_pred()) {
                            Tac* tac = pre->get_tac_list().back();
                            if(tac != nullptr) {
                                if(tac->getOpcode() == Type::BR || tac->getOpcode() == Type::CALL) {
                                    ((Label*)(tac->getSrc0()))->setPos(cmaxTacID - 1);
                                } else if(tac->getOpcode() == Type::BLBC || tac->getOpcode() == Type::BLBS) {
                                    ((Label*)(tac->getSrc1()))->setPos(cmaxTacID - 1);
                                }
                            }
                        }
                    }
                    if(everOnWorkList.find(succ) == everOnWorkList.end()) {
                        everOnWorkList.insert(succ);
                        workList.push(succ);
                    }
                }
            }
        }
    }
}

Variable* ConvertSSA::gen_name(Variable* var) {
    std::string old_name = var->getName().substr(0, var->getName().find("$"));
    if(varMap.find(var) == varMap.end()) {
        printf("Error: No Such Var In VarMap: ");
        std::cout << var->getName() << std::endl;
        return nullptr;
    }
    int vid = varMap[var];
    int i = varCount[vid];
    varCount[vid]++;
    std::string new_name = old_name + "$" + std::to_string(i);
    Variable* op = new Variable(new_name, var->getOffset(), true);
    curProc->get_sym().insert({new_name, op});
    varStack[vid].push(op);
    // printf("%d - size: %d\n", vid, varStack[vid].size());
    return op;
}

void ConvertSSA::rename_variables(CFGBlock* blk) {
    if(visited[blk->get_index()]) return;
    // printf("Visiting %d\n", blk->get_id());
    visited[blk->get_index()] = true;
    auto& tl = blk->get_tac_list();
    auto it = tl.begin();
    for(; it != tl.end(); ) {
        Tac* tac = *it;
        if(tac->getOpcode() != Type::PHI) break;
        Variable* src = (Variable*)tac->getSrc0();
        Operand* op = gen_name(src);
        Tac* mov = new Tac(cmaxTacID++, Type::MOVE, std::string("move"), tac->getDest(), op); //添加move指令
        curProc->get_sym().insert({"(" + std::to_string(mov->getTacID()) + ")", mov->getDest()});
        it++;
        tl.insert(it, mov); //在it位置插入mov，并指向下一个tac
        Tac* suc_tac = *it;
        Tac* pre_tac = (*it)->getPreTac();
        pre_tac->setSucTac(mov);
        suc_tac->setPreTac(mov);
        mov->setPreTac(pre_tac);
        mov->setSucTac(suc_tac);
        // if(PASS_DEBUG) {
        //     printf("INFO: Insert MOVE Function.\n"); //debug
        // }
    }
    for(; it != tl.end(); it++) {
        Tac* tac = *it;
        Operand *src0, *src1;
        // if(tac->getSrcSize() < 1) continue;
        src0 = tac->getSrc0(); //RHS
        if(src0 != nullptr && src0->getType() == OperandType::VAR) { //只需要处理Variable
            // if(PASS_DEBUG) {
            //     printf("INFO: Rename Variable.\n");
            // }
            tac->rebindSrc0(varStack[varMap[(Variable*)src0]].top());
        }
        // if(tac->getSrcSize() < 2) continue;
        src1 = tac->getSrc1(); //LHS(MOVE) or RHS
        if(src1 != nullptr && src1->getType() == OperandType::VAR) {
            // if(PASS_DEBUG) {
            //     printf("INFO: Rename Variable.\n");
            // }
            if(tac->getOpcode() == Type::MOVE) {
                Operand* op = gen_name((Variable*)src1);
                tac->rebindSrc1(op);
            } else {
                tac->rebindSrc1(varStack[varMap[(Variable*)src1]].top());
            }
        }
    }
    for(auto& succ: blk->get_succ()) {
        if(succ == nullptr) continue;
        for(auto& tac: succ->get_tac_list()) {
            if(tac->getOpcode() == Type::PHI) {
                Variable* old_var = (Variable*)(tac->getSrc0());
                tac->addSrc(varStack[varMap[old_var]].top());
            }
            // tac->dump();
            // old_var->dump();
            // tac->dump();
            // printf("var index:%d\n", varMap[old_var]);
            // printf("size:     %d\n", varStack[varMap[old_var]].size());
        }
    }
    for(auto& succ: blk->get_succ()) {
        if(succ == nullptr) continue;
        rename_variables(succ);
    }
    for(auto& tac: blk->get_tac_list()) {
        if(tac->getOpcode() == Type::MOVE) {
            Operand* op = tac->getSrc1();
            if(op == nullptr) {
                printf("Error: Should Not Be Empty!\n");
                return;
            }
            if(op->getType() == OperandType::VAR) {
                Variable* var = (Variable*)op;
                std::string old_name = var->getName().substr(0, var->getName().find("$"));
                Variable* old_var = (Variable*)(curProc->get_sym()[old_name]);
                varStack[varMap[old_var]].pop();
            }
        }
    }
}
