#include<stdio.h>
#include<iostream>
#include<string>
#include"tac.h"

#define CSSidlen 16

// FILE *fp;
Program pg;
char ch, chars[CSSidlen];

dt_ulong Number() { //Constants范围：uint64_t
    dt_ulong ans = 0;
    while(('0' <= ch) && (ch <= '9') && ((0x8000000000000000ULL + '0' - ch) / 10 >= ans)) {
        ans = ans*10 + ch - '0';
        ch = getc(stdin);
    }
    if(('0' <= ch) && (ch <= '9')) printf("Error: Number Too Large!\n");
    return ans;
}

std::string Identifier() {
    int i = 0;
    // chars[i++] = ch;
    while((('a' <= ch) && (ch <= 'z')) || (('A' <= ch) && (ch <= 'Z')) || (('0' <= ch) && (ch <= '9')) || (ch == '_')) {
        if(i < CSSidlen) chars[i] = ch;
        i++;
        ch = getc(stdin);
    }
    if(i >= CSSidlen) {
        printf("Error: Identifier Too Long!\n");
        return " ";
    }
    chars[i] = 0;
    return chars;
}

Operand* parse_operand() {
    Operand *operand;
    Operand *find_result;
    int number;
    //use getc() to parse operands like css.c
    ch = getc(stdin);
    switch(ch) {
        case '(': //Register_names
            ch = getc(stdin);
            number = Number();
            find_result = pg.findOperand("(" + std::to_string(number) + ")");
            if(find_result != nullptr) operand = find_result;
            else {
                operand = new Register(number);
                pg.insertOperand("(" + std::to_string(number) + ")", operand);
            }
            //')' will be read by Number()
            ch = getc(stdin); //' ' or '\n'
            break;
        case '[': //Instruction_labels
            ch = getc(stdin);
            operand = new Label(Number());
            //']' will be read by Number()
            ch = getc(stdin); //' ' or '\n'
            break;
        case 'F': //FP, Frame Pointer
            ch = getc(stdin); //'P'
            operand = new Address(TYPE_FP);
            ch = getc(stdin); //' ' or '\n'
            break;
        case 'G': //GP, Global Pointer
            ch = getc(stdin); //'P'
            operand = new Address(TYPE_GP);
            ch = getc(stdin); //' ' or '\n'
            break;
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9': //Constants
            operand = new Constant(Number());
            break;
        default: //Address_offsets or Field_offsets or Local_variables
            std::string name = Identifier();
            // ch = getc(stdin);
            if(ch == '#') {
                dt_long addr;
                scanf("%ld", &addr);
                operand = new Address(TYPE_NO, addr, name);
            } else if(ch == '@') {
                dt_long offset;
                scanf("%ld", &offset);
                find_result = pg.findOperand(name);
                if(find_result != nullptr) operand = find_result;
                else {
                    operand = new Variable(name, offset);
                    pg.insertOperand(name, operand);
                }
            }
            ch = getc(stdin); //' ' or '\n'
    }
    return operand;
}

void parse_tac_file() {
    std::string c;
    dt_ulong TacID;
    std::string op;
    Scope *scope = pg.insertScope(); //global scope
    while(std::cin >> c) { //"instr"
        // printf("%s\n", c.c_str());
        scanf("%ld", &TacID);
        // printf("%ld\n", TacID);
        ch = getc(stdin); //':'
        // printf("%c\n", ch);
        std::cin >> op;
        ch = getc(stdin); //' '
        // printf("%s\n", op.c_str());
        int opcode = pg.get_opcode(op);
        // printf("%d\n", opcode);
        Tac *tac = nullptr;
        Operand *src0, *src1;
        switch(opcode) {
            case ADD: case SUB: case MUL: case DIV:
            case MOD: case CMPEQ: case CMPLE: case CMPLT: 
            case BLBC: case BLBS: case STORE: case MOVE:
                src0 = parse_operand();
                src1 = parse_operand();
                tac = new Tac(TacID, opcode, op, src0, src1);
                scope->insertTac(tac);
                break;
            case NEG: case BR: case CALL: 
            case PARAM: case WRITE: case LOAD:
                src0 = parse_operand();
                tac = new Tac(TacID, opcode, op, src0);
                scope->insertTac(tac);
                break;
            case ENTER:
                src0 = parse_operand();
                tac = new Tac(TacID, opcode, op, src0);
                scope = pg.insertScope();
                scope->insertTac(tac);
                break;
            case RET:
                src0 = parse_operand();
                tac = new Tac(TacID, opcode, op, src0);
                scope->insertTac(tac);
                scope = pg.popScope();
                break;
            case READ: case WRL: case NOP:
                tac = new Tac(TacID, opcode, op);
                scope->insertTac(tac);
                break;
            case ENTRYPC: 
                tac = new Tac(TacID, opcode, op);
                scope->insertTac(tac);
                pg.setMainScope();
                // scope = pg.insertScope();
                break;
            default: 
                printf("Error: Opcode Not Recogonized!\n");
                return;
        }
        pg.insertOperand("(" + std::to_string(tac->getTacID()) + ")", tac->getDest());
        // tac->dump();
    }
    // printf("TAC File Parsed Successfully.\n");
    return;
}

void Tac::toC() {
    std::cout << "\tL" << TacID << ": ";
    if(getDUType() == SDEF || getDUType() == DDEF) {
        std::cout << "long ";
        dest->toC();
        if(opcode == READ) std::cout << "; ";
        else std::cout << " = ";
    }
    switch(opcode){
        case ADD:       src0->toC(); std::cout << " + "; src1->toC(); break;
        case SUB:       src0->toC(); std::cout << " - "; src1->toC(); break;
        case MUL:       src0->toC(); std::cout << " * "; src1->toC(); break;
        case DIV:       src0->toC(); std::cout << " / "; src1->toC(); break;
        case MOD:       src0->toC(); std::cout << " % "; src1->toC(); break;
        case NEG:       std::cout << " - "; src0->toC(); break;
        case CMPEQ:     std::cout << "("; src0->toC(); std::cout << " == "; src1->toC();std::cout << ")";  break;
        case CMPLE:     std::cout << "("; src0->toC(); std::cout << " <= "; src1->toC();std::cout << ")";  break;
        case CMPLT:     std::cout << "("; src0->toC(); std::cout << " < "; src1->toC();std::cout << ")";  break;
        case BR:        std::cout << "goto L"; src0->toC(); break;
        case BLBC:      std::cout << "if(!"; src0->toC(); std::cout << ") goto L"; src1->toC(); break;
        case BLBS:      std::cout << "if("; src0->toC(); std::cout << ") goto L"; src1->toC(); break;
        case CALL:      std::cout << "FP--; func"; src0->toC(); std::cout << "()"; break;
        case LOAD:      std::cout << "*((long*)"; src0->toC(); std::cout << ")"; break;
        case STORE:     std::cout << "*((long*)"; src1->toC(); std::cout << ") = "; src0->toC(); break;
        case MOVE:      src1->toC(); std::cout << " = "; src0->toC(); break;
        case READ:      std::cout << "ReadLong("; dest->toC(); std::cout << ")";  break;
        case WRITE:     std::cout << "WriteLong("; src0->toC(); std::cout << ")"; break;
        case WRL:       std::cout << "WriteLine()"; break;
        case PARAM:     std::cout << "*(FP--) = "; src0->toC(); break;
        case RET:       std::cout << "FP++; FP += " << (((Constant*)src0)->num)/8; break;
        default:break;
    }
    std::cout << ";" << std::endl;
}

void Scope::toC() {
    if(ScopeID == 0)
        return;
    std::string func_name = "";
    if(MainScope)
        func_name = "main";
    else
        func_name = "func" + std::to_string(func_id);
    std::cout << "void " << func_name << "() {" << std::endl;
    //声明局部变量
    // for(auto& kv: sym)
    //     if(kv.second->getType() == OperandType::VAR){
    //         std::cout << "long ";
    //         kv.second->toC();
    //         std::cout << ";" << std::endl;
    //     }
    //翻译tac
    for(Tac* tac = Tac_head; tac != nullptr; tac = tac->getSucTac())
        tac->toC();
    std::cout << "}" << std::endl;
}

void Program::toC() {
    std::cout << "#include <stdio.h>" << std::endl;
    std::cout << "#define WriteLine() printf(\"\\n\");" << std::endl;
    std::cout << "#define WriteLong(x) printf(\" %lld\", (long)x);" << std::endl;
    std::cout << "#define ReadLong(a) if (fscanf(stdin, \"%lld\", &a) != 1) a = 0;" << std::endl;
    std::cout << "#define long long long" << std::endl;
    std::cout << "long GP[1048576];" << std::endl;  // 2^20 * 8Byte = 8MB
    std::cout << "long* FP = GP + 1048500;" << std::endl;
    for(auto& scope: Scopes)
        scope->toC();
}