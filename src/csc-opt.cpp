#include<stdio.h>
#include<iostream>
#include<string>
#include"tac.h"

// FILE *fp;
Program pg;
char ch;

dt_ulong Number() { //根据定义读取的只可能是uint64_t
    dt_ulong ans = 0;
    while(('0' <= ch) && (ch <= '9') && ((0x8000000000000000ULL + '0' - ch) / 10 >= ans)) {
        ans = ans*10 + ch - '0';
        ch = getc(stdin);
    }
    if(('0' <= ch) && (ch <= '9')) printf("Error: Number Too Large!\n");
    return ans;
}

Operand* parse_operand() {
    Operand *operand;
    //use getc() to parse operands like css.c
    ch = getc(stdin);
    switch(ch) {
        case '(': //Register_names
            ch = getc(stdin);
            operand = new Register(Number());
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
            ch = getc(stdin);
            //TODO:
            operand = new Address(TYPE_NO);
    }
    return operand;
}

void parse_tac_file() {
    std::string c;
    dt_ulong TacID;
    std::string op;
    while(std::cin >> c) {
        std::cin >> TacID;
        std::cin >> op;
        int opcode = pg.get_opcode(op);
        switch(opcode) {
            case ADD: case SUB: case MUL: case DIV:
            case MOD: case CMPEQ: case CMPLE: case CMPLT: 
            case BLBC: case BLBS: case STORE: case MOVE:
                Operand *src0 = parse_operand();
                Operand *src1 = parse_operand();
                Tac *tac = new Tac(TacID, opcode, src0, src1);

                break;
            case NEG: case BR: case CALL: case LOAD:
            case PARAM: case ENTER: case RET: case WRITE:
                Operand *src0 = parse_operand();
                Tac *tac = new Tac(TacID, opcode, src0);

                break;

            case READ: case WRL: case ENTRYPC:
                Tac *tac = new Tac(TacID, opcode);

                break;
            case NOP:
                break;
            default: 
                printf("Error: Opcode Not Recogonized!\n");
                return;
        }
    }
    printf("TAC File Parsed Successfully.\n");
    return;
}

int main(int argc, char *argv[]) {
    
    if(argc < 2) {
        printf("Usage: ./csc-opt xxx.tac\n");
        return 1;
    } else {
        // fp = fopen(argv[1], "r");
        freopen(argv[1], "r", stdin);
    }
    parse_tac_file();

    return 0;
}
