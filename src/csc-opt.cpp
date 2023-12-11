#include<stdio.h>
#include<iostream>
#include<string>
#include"tac.h"

FILE *fp;
Program pg;

Operand* parse_operand() {
    Operand *operand;
    //TODO: use getc() to parse operands like css.c
    //GP or FP or Constants or Address_offsets or 
    //Field_offsets or Local_variables or Register_names or
    //Instruction_labels

    return operand;
}

void parse_tac_file() {
    std::string c;
    long TacID;
    std::string op;
    while(std::cin >> c) {
        std::cin >> TacID;
        std::cin >> op;
        int opcode = pg.get_opcode(op);
        switch(opcode) {
            case ADD:
            case SUB:
            case MUL:
            case DIV:
            case MOD:
            case NEG: {
                Operand *src0 = parse_operand();
                Operand *src1 = parse_operand();
                Tac *tac = new Tac(TacID, opcode, src0, src1);

                break;
            }
            default: {
                printf("Error: Opcode Not Recogonized!\n");
                return;
            }
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
        fp = fopen(argv[1], "r");
    }
    parse_tac_file();

    return 0;
}
