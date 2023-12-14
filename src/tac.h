#include<vector>
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
    virtual void Operand::dump() = 0;
    virtual void Operand::toC() = 0;
};

class Constant: public Operand { //Constants
public:
    dt_ulong num = 0; //数值常量

    Constant::Constant(dt_ulong num) {
        this->num  = num;
    }

    void Constant::dump() {
        printf("%ld", num);
    }
};

class Address: public Operand { //Address_offsets or Field_offsets
public:
    int type = 0;
    dt_long addr = 0; //地址、偏移
    std::string name;

    Address::Address(int type, dt_long addr=0, std::string name="") {
        this->type = type;
        this->addr = addr;
        this->name = name;
    }

    void Address::dump() {
        if(type == TYPE_FP) printf("FP");
            else if(type == TYPE_GP) printf("GP");
            else printf("%s#%ld", name, addr);
    }
};

class Register: public Operand { //Register_names
public:
    dt_ulong reg_index = 0; //虚拟寄存器

    Register::Register(dt_ulong RegID) {
        this->reg_index = RegID;
    }

    void Register::dump() {
        printf("(%ld)", reg_index);
    }
};

class Variable: public Operand { //Local_variables
public:
    std::string name;
    dt_long offset = 0;

    Variable::Variable(std::string name, dt_long offset) {
        this->name = name;
        this->offset = offset;
    }

    void Variable::dump() {
        printf("%s#%ld", name, offset);
    }
};

class Label: public Operand { //Instruction_labels
public:
    dt_ulong instr_pos = 0; //指令标签

    Label::Label(dt_ulong TacID) {
        this->instr_pos = TacID;
    }

    void Label::dump() {
        printf("[%ld]", instr_pos);
    }
};

class Tac {
private:
    dt_ulong TacID = 0;
    Type opcode = ADD;
    Operand *dest;
    Operand *src0;
    Operand *src1;
public:
    Tac::Tac(dt_ulong TacID, int opcode, Operand *src0=NULL, Operand *src1=NULL) {
        this->TacID  = TacID;
        this->opcode = Type(opcode);
        this->src0   = src0;
        this->src1   = src1;
        this->dest   = new Register(TacID);
    };

    void Tac::dump() {
        printf("instr %ld: %d ", TacID, opcode);
        if(src0 != NULL) {
            src0->dump();
        }
        if(src1 != NULL) {
            src1->dump();
        }
        printf("\n");
    }
};

class Scope {
public:
    std::vector<Tac*> Tacs;

    Scope::Scope() {
        Tacs.clear();
    }
    
    void Scope::dump() {
        for(auto it = Tacs.begin(); it != Tacs.end(); it++) {
            (*it)->dump();
        }
        printf("\n");
    }
};

class Program {
public:
    std::vector<Scope*> Scopes;
    std::unordered_map<std::string, int> mp;

    Program::Program() {
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

    int Program::get_opcode(std::string op) {
        return mp[op];
    }

    void Program::dump() {
        for(auto it = Scopes.begin(); it != Scopes.end(); it++) {
            (*it)->dump();
        }
    }
};
