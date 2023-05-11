#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#define MOVE 0
#define ADD 1
#define SUB 2
#define BRA 3
#define CMP 4
#define BEQ 5
#define BNE 6
#define EXG 7   /*EXG exchanges the contents of two registers */
#define STOP 15

void main (void)
{
//1
unsigned short int PC = 0; /* program counter */
unsigned short int DO = 0; /* data register */
unsigned short int AO = 0; /* address register */
unsigned short int CCR = 0; /* condition code register */
unsigned short int MAR; /* memory address register */
unsigned short int MBR; /* memory buffer register */
unsigned short int IR; /* instruction register */
unsigned short int operand; /* the 8-bit operand from the IR */
unsigned short int source; /* source operand */
//2
unsigned short int destination; /* the destination value */
unsigned short int opcode; /* the 4-bit op-code from the IR */
unsigned short int amode; /* the 2-bit addressing mode */
unsigned short int direction; /* the 1-bit data direction flag */
unsigned short int memory[]= {0x05,0x04,0xF0,0x0}; /*the memory *//* the memory */
unsigned short int run = 1; /* execute program while run is 1 */

/* Instruction format: */
/* 7 6 5 4 3 2 1 «90 */
/* Bit 1 and 0 2-bit address mode */
/* 00 address mode = absolute */
/* 01 address mode = literal */
/* 10 address mode = indexed */
/* 11 address mode = relative */
/* Bit 2 l-bit direction (source/operand) */
/* Bit 3 not used */
/* Bit 7 to 4 4-bit instruction code */
/* main loop */

//3

while (run)
{
    MAR = PC; /* PC to MAR */
    PC = PC + 1; /* increment PC */
    MBR = memory[MAR]; /* get next instruction */
    IR = MBR; /* copy MBR to IR */
    opcode = IR; /* store the op-code bits */

    MAR = PC; /* PC to MAR */
    PC = PC + 1; /* increment PC */
    MBR = memory [MAR]; /* get the operand */
    IR = MBR; /* copy MBR to IR */
    operand = IR; /* store the operand bits */
    amode = opcode & 0x03; /* extract the address mode bits */
    direction = (opcode & 0x04) >> 2; /* get data direction 0 = register to memory
    1 = memory to register */
    opcode = opcode >> 4; /* get the 4-bit instruction code */
    /* use the address mode to get the source operand */

//4
    switch (amode)
    {
        case 0: {source = memory[operand];      break;} /* absolute */
        case 1: {source = operand;              break;} /* literal */
        case 2: {source = memory[AO + operand]; break;} /* indexed */
        case 3: {source = memory[PC + operand]; break;} /* PC relative */
    }

/* now execute the instruction */
switch (opcode)
{
    case MOVE: {if (direction == 0) destination = DO;
                else DO = source;
                if (DO == 0) CCR = 1;
                else CCR = 0; /* update CCR */
                //printf("%d\n", DO);
        break;
    }
    case ADD: {if (direction == 0){
                    destination = DO + source;
                if (destination == 0) CCR = 1; else CCR = 0;
                }
                else{
                    DO = DO + source;
                    if (DO == 0 ) CCR = 1; else CCR = 0;
                }
                //printf("%d\n", DO);
                break;
    }
//5
    case SUB: {if (direction == 0){
                destination = DO - source;
                if (destination == 0) CCR = 1; else CCR = 0;
            }
            else{
                DO = DO - source;
                if (DO == 0 ) CCR = 1; else CCR = 0;
            }
            //printf("%d\n", DO);
            break;
    }
    case BRA: { if (amode == 0) PC = operand;
                if (amode == 1) PC = PC + operand; break;
    }
    case CMP: { MBR = DO - source;
                if (MBR == 0) CCR = 1;
                else CCR = 0; break;
    }
    case BEQ: {if (CCR == 1){
                    if (amode == 0) PC = operand;
                    if (amode == 1) PC = PC + operand;
                }
            break;
    }
//6
    case BNE: {if (CCR != 1){
                if (amode == 0) PC = operand;
                if (amode == 1) PC = PC + operand;
                }
         break;
    }
    case EXG: {MBR = DO; DO = AO; AO = MBR;
         break;
    }
    case STOP: {run = 0;
            break;
    }
}
//7
/* save result in memory if register to memory */
    if (direction == 0)
        switch (amode)
        { case 0: { memory[operand] = destination; break; /* absolute */
          }
        case 1: { break; /* literal */
        }
        case 2: { memory[AO + operand] = destination; break; /* indexed */
        }
        case 3: { memory[PC + operand] = destination; break; /* PC relative */
        }
        }
}
    printf("%d", DO);
}
