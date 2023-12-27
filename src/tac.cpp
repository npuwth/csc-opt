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
        pg.maxTacID = TacID;
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
