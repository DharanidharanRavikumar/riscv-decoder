#include "decoder.h"

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

int main(void)
{
    uint32_t inp_inst;

    printf("========================================\n");
    printf("       RISC-V RV32I INSTRUCTION DECODER\n");
    printf("========================================\n");

    printf("\nEnter a 32-bit instruction in hexadecimal.\n");
    printf("Example: 0x003100B3\n");
    printf("Enter 0 to exit.\n\n");

    while (1)
    {
        printf("Instruction > 0x");

        if (scanf("%" SCNx32, &inp_inst) != 1)
        {
            printf("Invalid hexadecimal input.\n");
            return 1;
        }

        if (inp_inst == 0)
        {
            printf("\nExiting decoder...\n");
            break;
        }

        decode_instruction resultobj = decoder(inp_inst);

        printf("\n----------------------------------------\n");
        printf("           DECODED INSTRUCTION\n");
        printf("----------------------------------------\n");

        printf("Original Raw instruction : 0x%08" PRIX32 "\n",resultobj.og_inp_inst);
        printf("Instruction     : %s\n",resultobj.instruction_name);
        printf("Type            : %s\n",type_name(resultobj.type));
        printf("Opcode          : 0x%02X\n",resultobj.opcode);
        printf("rd              : x%d\n",resultobj.rd);
        printf("rs1             : x%d\n",resultobj.rs1);
        printf("rs2             : x%d\n",resultobj.rs2);
        printf("funct3          : 0x%02X\n",resultobj.func3);
        printf("funct7          : 0x%02X\n",resultobj.func7);
        printf("Immediate       : %d\n",resultobj.immediate);
        printf("shamt           : %d\n", resultobj.shamt);

        printf("----------------------------------------\n\n");
    }

    return 0;
}