#ifndef __PASS_H__
#define __PASS_H__
#include "cfg.h"

//优化类型
enum class PassType :int
{
    SimpleConstantPropagation,              //简单常量传播
    DeadStatementElimination,               //死代码删除
    ConvertSSA,                             //转为SSA形式
    RevertSSA,                              //从SSA转回原形式
    CommonSubexpressionElimination,         //公共子表示式删除
    LoopInvariantCodeMotion,                //循环不变代码移动
};

class Pass
{
private:
    CFGProgram* m_cfg;  //pass执行对象
public:
    Pass(CFGProgram* i_cfg): m_cfg(i_cfg) {}
    virtual void run() = 0;     //run函数，由继承的子类实现
};

class Optimizer
{
private:
    CFGProgram* m_cfg{nullptr};
    std::vector<Pass*> m_passes{};
public:
    Optimizer(CFGProgram* i_cfg): m_cfg(i_cfg){}
    void add_pass(PassType pass_type);  //添加一个pass
    void run_pass();                    //执行所有pass
};

#endif