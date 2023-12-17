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
    NOP
} Type;

class Operand {
public:
    virtual void dump() = 0;
    virtual void toC() = 0;
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
};

class Tac {
private:
    dt_ulong TacID = 0;
    Type opcode = ADD;
    std::string opname;
    Operand *dest = nullptr;
    Operand *src0 = nullptr;
    Operand *src1 = nullptr;
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

    Operand* getSrc0() {
        return this->src0;
    }

    Operand* getSrc1() {
        return this->src1;
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
};

class Scope {
public:
    dt_ulong ScopeID = 0;
    //0为global scope
    std::vector<Tac*> Tacs;

    Scope(dt_ulong ScopeID) {
        Tacs.clear();
        this->ScopeID = ScopeID;
    }

    void insertTac(Tac *tac) {
        Tacs.push_back(tac);
    }
    
    void dump() {
        for(auto it = Tacs.begin(); it != Tacs.end(); it++) {
            (*it)->dump();
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
    std::stack<Scope*> ScopeStack;
    std::unordered_map<std::string, int> mp;
    dt_ulong ScopeID = 0;

    Program() {
        Scopes.clear();
        while(!ScopeStack.empty()) ScopeStack.pop();
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
    }

    int get_opcode(std::string op) {
        return mp[op];
    }

    Scope* insertScope() {
        Scope *scope = new Scope(ScopeID++);
        Scopes.push_back(scope);
        ScopeStack.push(scope);
        return scope;
    }
    
    Scope* popScope() {
        ScopeStack.pop();
        return ScopeStack.top();
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
