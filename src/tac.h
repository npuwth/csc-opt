#include<vector>
#include<string>
#include<unordered_map>

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
};

class Variable: public Operand {
public:
    long num = 0;   //数值或地址
    bool is_FP = 0;
    bool is_GP = 0;

    void Variable::dump() {
        if(is_FP) printf("FP");
        else if(is_GP) printf("GP");
        else printf("%ld", num);
    }
};

class Register: public Operand {
public:
    long reg_index = 0; //虚拟寄存器数量无限

    Register::Register(long TacID) {
        reg_index = TacID;
    }

    void Register::dump() {
        printf("(%ld)", reg_index);
    }
};

class Label: public Operand {
public:
    long instr_pos = 0;

    void Label::dump() {
        printf("[%ld]", instr_pos);
    }
};

class Tac {
private:
    long TacID = 0;
    Type opcode = ADD;
    Operand *dest;
    Operand *src0;
    Operand *src1;
public:
    Tac::Tac(long TacID, int opcode, Operand *src0, Operand *src1) {
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
