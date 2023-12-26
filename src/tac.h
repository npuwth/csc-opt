#ifndef __TAC_H__
#define __TAC_H__
#include<vector>
#include<stack>
#include<string>
#include<unordered_map>

typedef int64_t dt_long;
typedef uint64_t dt_ulong;

#define TYPE_NO 0 //normal address
#define TYPE_FP 1 //fp address
#define TYPE_GP 2 //gp address

typedef enum {
//Arithmetic
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    NEG,
    CMPEQ, //equality
    CMPLE, //less than or equal
    CMPLT, //less than
//Branch
    BR,
    BLBC,
    BLBS,
    CALL,
//Data Movement
    LOAD,
    STORE,
    MOVE,
//IO
    READ,
    WRITE,
    WRL,
//Others
    PARAM,
    ENTER, //denote function begin
    RET,   //denote function return
    ENTRYPC, //denote main function
    NOP,
    PHI
} Type;

typedef enum {
    CONST,
    ADDR,
    REG,
    VAR,
    LABEL
} OperandType;

typedef enum {
    NDEF,   //无定值
    SDEF,   //单定值
    DDEF    //双定值
} DUType;

class Operand {
public:
    int oper_id{-1};
    std::vector<int> def_id;    // 到达定值分析中对应的定值列表
    Operand() {def_id.clear();}
    virtual void dump() = 0;
    virtual void toC() = 0;
    virtual OperandType getType() = 0;
};

class Constant: public Operand { //Constants
public:
    dt_ulong num = 0; //数值常量

    Constant(dt_ulong num) {
        this->num  = num;
    }

    void dump() {
        printf("%ld", num);
    }

    void toC () {
        ;
    }

    OperandType getType() {
        return OperandType::CONST;
    }
};

class Address: public Operand { //Address_offsets or Field_offsets
public:
    int type = 0;
    dt_long addr = 0; //地址、偏移
    std::string name;

    Address(int type, dt_long addr=0, std::string name="") {
        this->type = type;
        this->addr = addr;
        this->name = name;
    }

    void dump() {
        if(type == TYPE_FP) printf("FP");
            else if(type == TYPE_GP) printf("GP");
            else printf("%s#%ld", name.c_str(), addr);
    }

    void toC () {
        ;
    }

    OperandType getType() {
        return OperandType::ADDR;
    }
};

class Register: public Operand { //Register_names
public:
    dt_ulong reg_index = 0; //虚拟寄存器

    Register(dt_ulong RegID) {
        this->reg_index = RegID;
    }

    void dump() {
        printf("(%ld)", reg_index);
    }

    void toC () {
        ;
    }

    OperandType getType() {
        return OperandType::REG;
    }
};

class Variable: public Operand { //Local_variables
public:
    std::string name;
    dt_long offset = 0;

    Variable(std::string name, dt_long offset) {
        this->name = name;
        this->offset = offset;
    }

    void dump() {
        printf("%s@%ld", name.c_str(), offset);
    }

    void toC () {
        ;
    }

    OperandType getType() {
        return OperandType::VAR;
    }
};

class Label: public Operand { //Instruction_labels
public:
    dt_ulong instr_pos = 0; //指令标签

    Label(dt_ulong TacID) {
        this->instr_pos = TacID;
    }

    void dump() {
        printf("[%ld]", instr_pos);
    }

    void toC () {
        ;
    }

    OperandType getType() {
        return OperandType::LABEL;
    }
};

class Tac {
private:
    dt_ulong TacID = 0;
    Type opcode = ADD;
    std::string opname;
    Operand *dest = nullptr;
    Operand *src0 = nullptr;
    Operand *src1 = nullptr;
    Tac *preTac;
    Tac *sucTac;
public:
    Tac(dt_ulong TacID, int opcode, std::string opname, Operand *src0=nullptr, Operand *src1=nullptr) {
        this->TacID  = TacID;
        this->opcode = Type(opcode);
        this->opname = opname;
        // printf("judge: %d\n", src0 == nullptr);
        this->src0   = src0;
        this->src1   = src1;
        this->dest   = new Register(TacID);
    };

    dt_ulong getTacID() {
        return this->TacID;
    }

    int getOpcode() {
        return this->opcode;
    }

    Operand* getDest() {
        return this->dest;
    }

    Operand* getSrc0() {
        return this->src0;
    }

    Operand* getSrc1() {
        return this->src1;
    }

    void rebindOp(Type new_opcode, std::string new_opname) {
        this->opcode = new_opcode;
        this->opname = new_opname;
    }

    void rebindDest(Operand* new_dest) { 
        this->dest = new_dest; 
    }

    void rebindSrc0(Operand* new_src0) { 
        this->src0 = new_src0; 
    }

    void rebindSrc1(Operand* new_src1) { 
        this->src1 = new_src1; 
    }

    void connectTac(Tac *tac) {
        this->sucTac = tac;
        tac->preTac = this;
    }

    Tac* getSucTac() {
        return this->sucTac;
    }

    Tac* getPreTac() {
        return this->preTac;
    }

    void setSucTac(Tac* suc_tac) {
        this->sucTac = suc_tac;
    }

    void setPreTac(Tac* pre_tac) {
        this->preTac = pre_tac;
    }

    void dump() {
        printf("\tinstr %ld: %s ", TacID, opname.c_str());
        if(src0 != nullptr) src0->dump();
        printf(" ");
        if(src1 != nullptr) src1->dump();
        printf("\n");
    }

    void toC() {
        ;
    }

    //定值类型的判断，用于在到达定值和活跃变量分析中进行分类处理
    DUType getDUType() {
        switch(this->opcode)
        {
            //无定值
            case Type::BR: case Type::BLBC: case Type::BLBS: case Type::CALL:
            case Type::STORE: case Type::WRITE: case Type::WRL: case Type::PARAM:
            case Type::ENTER: case Type::RET: case Type::NOP: case Type::ENTRYPC:
                return NDEF;
                break;
            //MOVE有两个定值
            case Type::MOVE:
                return DDEF;
                break;
            //单一的对dest定值
            default:
                return SDEF;
                break;
        } 
    }
};

class Scope {
public:
    dt_ulong ScopeID = 0;
    //ScopeID=0为global scope
    //其他为local scope（函数）
    // std::vector<Tac*> Tacs;
    Tac *Tac_head;
    Tac *Tac_cur;
    bool MainScope = 0; //main function
    std::unordered_map<std::string, Operand*> sym; //symbol table

    Scope(dt_ulong ScopeID, bool MainScope) {
        // Tacs.clear();
        Tac_head = nullptr;
        Tac_cur  = nullptr;
        this->ScopeID = ScopeID;
        this->MainScope = MainScope;
    }

    void insertTac(Tac *tac) {
        // Tacs.push_back(tac);
        if(Tac_cur != nullptr) {
            Tac_cur->connectTac(tac);
            Tac_cur = tac;
        } else {
            Tac_head = tac;
            Tac_cur  = tac;
        }
    }

    void removeTac(Tac *tac) {
        //TODO
        ;
    }
    
    void dump() {
        Tac *p = this->Tac_head;
        while(p != nullptr) {
            p->dump();
            p = p->getSucTac();
        }
        printf("\n");
    }

    void toC() {
        ;
    }
};

class Program {
public:
    std::vector<Scope*> Scopes;
    std::vector<Scope*> ScopeStack;
    dt_ulong stackTop = 0;
    std::unordered_map<std::string, int> mp;
    dt_ulong ScopeID = 0;
    bool MainScope = 0;

    Program() {
        Scopes.clear();
        // while(!ScopeStack.empty()) ScopeStack.pop();
        ScopeStack.clear();
        stackTop = 0;
        mp["add"] = ADD;
        mp["sub"] = SUB;
        mp["mul"] = MUL;
        mp["div"] = DIV;
        mp["mod"] = MOD;
        mp["neg"] = NEG;
        mp["cmpeq"] = CMPEQ;
        mp["cmple"] = CMPLE;
        mp["cmplt"] = CMPLT;
        mp["br"] = BR;
        mp["blbc"] = BLBC;
        mp["blbs"] = BLBS;
        mp["call"] = CALL;
        mp["load"] = LOAD;
        mp["store"] = STORE;
        mp["move"] = MOVE;
        mp["read"] = READ;
        mp["write"] = WRITE;
        mp["wrl"] = WRL;
        mp["param"] = PARAM;
        mp["enter"] = ENTER;
        mp["ret"] = RET;
        mp["entrypc"] = ENTRYPC;
        mp["nop"] = NOP;
        mp["phi"] = PHI;
    }

    int get_opcode(std::string op) {
        return mp[op];
    }

    void setMainScope() {
        this->MainScope = 1;
    }

    Scope* insertScope() {
        Scope *scope = new Scope(ScopeID++, MainScope);
        MainScope = 0;
        Scopes.push_back(scope);
        if(stackTop >= ScopeStack.size()) {
            ScopeStack.push_back(scope);
            stackTop++;
        } else {
            ScopeStack[stackTop++] = scope;
        }
        return scope;
    }
    
    Scope* popScope() {
        if(stackTop <= 1) {
            printf("Error: No Elements In ScopeStack!\n");
            return nullptr;
        }
        stackTop--;
        return ScopeStack[stackTop - 1];
    }

    Operand* findOperand(std::string name) {
        for(auto it = ScopeStack.rbegin(); it != ScopeStack.rend(); it++) {
            if((*it)->sym.find(name) != (*it)->sym.end()) return (*it)->sym[name];
        }
        return nullptr;
    }

    void insertOperand(std::string name, Operand* operand) {
        if(stackTop == 0) {
            printf("Error: No Elements In ScopeStack!\n");
            return;
        }
        ScopeStack[stackTop - 1]->sym[name] = operand;
    }

    void dump() {
        for(auto it = Scopes.begin(); it != Scopes.end(); it++) {
            (*it)->dump();
        }
    }
    
    void toC() {
        ;
    }
};

void parse_tac_file();
extern Program pg;

#endif