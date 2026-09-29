#include "decoder.h"
#include <stdio.h>
#include <stdint.h>




decode_instruction decoder(uint32_t inp_inst)
{
    decode_instruction result;

    result.og_inp_inst = inp_inst;
    result.opcode = inp_inst & 0x7F;

    switch (result.opcode)
    {
        case 0x33:
            return decode_RType(inp_inst);

        case 0x13:
            return decode_IType(inp_inst);

        case 0x03:
            return decode_IType(inp_inst);

        case 0x23:
            return decode_SType(inp_inst);

        case 0x63:
            return decode_BType(inp_inst);

        case 0x37:
            return decode_UType(inp_inst);

        case 0x17:
            return decode_UType(inp_inst);

        case 0x6F:
            return decode_JType(inp_inst);

        case 0x67:
            return decode_IType(inp_inst);

        default:
            result.type = TYPE_UNKNOWN;
            result.instruction_name = "UNKNOWN";

            return result;
    }
}

const char *type_name(InstructionType type)
{
    switch(type)
    {
        case TYPE_R:
            return "R";

        case TYPE_I:
            return "I";

        case TYPE_S:
            return "S";

        case TYPE_B:
            return "B";

        case TYPE_U:
            return "U";

        case TYPE_J:
            return "J";

        default:
            return "UNKNOWN TYPE";
    }
}



decode_instruction decode_RType(uint32_t inp_inst)
{
    decode_instruction result;

    result.og_inp_inst = inp_inst;
    result.opcode = inp_inst & 0x7F;

    result.rd = (inp_inst >> 7) & 0x1F;
    result.func3 = (inp_inst >> 12) & 0x07;
    result.rs1 = (inp_inst >> 15) & 0x1F;
    result.rs2 = (inp_inst >> 20) & 0x1F;
    result.func7 = (inp_inst >> 25) & 0x7F;
    result.type = TYPE_R;

    switch (result.func3)
    {
        case 0x0:

            if (result.func7 == 0x00)
            {
                result.instruction_name = "ADD";
            }
            else if (result.func7 == 0x20)
            {
                result.instruction_name = "SUB";
            }
            else
            {
                result.instruction_name = "UNKNOWN R";}
            break;

        case 0x1:
            result.instruction_name = "SLL";
            break;

        case 0x2:
            result.instruction_name = "SLT";
            break;

        case 0x3:
            result.instruction_name = "SLTU";
            break;

        case 0x4:
            result.instruction_name = "XOR";
            break;
            
        case 0x5:
            if (result.func7 == 0x00)
            {
                result.instruction_name = "SRL"; }
            else if (result.func7 == 0x20)
            {
                result.instruction_name = "SRA"; }
            else
            {
                result.instruction_name = "UNKNOWN R";}
            break;
            
        case 0x6:
            result.instruction_name = "OR";
            break;

        case 0x7:
            result.instruction_name = "AND";
            break;

        default:
            result.instruction_name = "UNKNOWN R";
            break;
    }
      return result;
}

decode_instruction decode_IType(uint32_t inp_inst)
{
    decode_instruction result;

    result.og_inp_inst = inp_inst;
    result.opcode = inp_inst & 0x7F;

    result.rd = (inp_inst >> 7) & 0x1F;
    result.func3 = (inp_inst >> 12) & 0x07;
    result.rs1 = (inp_inst >> 15) & 0x1F;

    result.rs2 = 0;

    result.func7 = (inp_inst >> 25) & 0x7F;
    result.immediate = (int32_t)inp_inst >> 20; // Extract the 12-bit I-type immediate and sign extend it

    result.type = TYPE_I;
    result.shamt = 0; //shift amount for slli, srli and srai

    if (result.opcode == 0x67) // for jalr instn's
    {
        result.instruction_name = "JALR";
        return result;
    }

    if (result.opcode == 0x03)// for load instn's
    {
        switch (result.func3)
        {
            case 0x0:
                result.instruction_name = "LB";
                break;

            case 0x1:
                result.instruction_name = "LH";
                break;

            case 0x2:
                result.instruction_name = "LW";
                break;

            case 0x4:
                result.instruction_name = "LBU";
                break;

            case 0x5:
                result.instruction_name = "LHU";
                break;

            default:
                result.instruction_name = "UNKNOWN LOAD";
                break;
        }

        return result;
    }

    switch (result.func3)//immd arithmetic and logical operations
    {
        case 0x0:
            result.instruction_name = "ADDI";
            break;
        case 0x1://slli
            result.shamt = inp_inst & 0x1F;
            result.instruction_name = "SLLI";
            break;
        case 0x2:
            result.instruction_name = "SLTI";
            break;
        case 0x3:
            result.instruction_name = "SLTIU";
            break;
        case 0x4:
            result.instruction_name = "XORI";
            break;
        case 0x5:// for slri or srai
            result.shamt = inp_inst & 0x1F;

            if (result.func7 == 0x00)
            {
                result.instruction_name = "SRLI";
            }
            else if (result.func7 == 0x20)
            {
                result.instruction_name = "SRAI";
            }
            else
            {
                result.instruction_name = "UNKNOWN I";}

            break;
        case 0x6:
            result.instruction_name = "ORI";
            break;
        case 0x7:
            result.instruction_name = "ANDI";
            break;
        default:
            result.instruction_name = "UNKNOWN I";
            break;
    }
    return result;
}
decode_instruction decode_SType(uint32_t inp_inst)
{
    decode_instruction result;
    result.og_inp_inst = inp_inst;

    result.opcode = inp_inst & 0x7F;
    result.func3 = (inp_inst >> 12) & 0x07;
    result.rs1 = (inp_inst >> 15) & 0x1F;
    result.rs2 = (inp_inst >> 20) & 0x1F;
    result.rd = 0;
    result.func7 = 0;
    result.shamt = 0;

    uint32_t imm_11_5 = (inp_inst >> 25) & 0x7F;
    uint32_t imm_4_0 = (inp_inst >> 7) & 0x1F;
    uint32_t immediate =
        (imm_11_5 << 5) |
        imm_4_0;

    result.immediate = (int32_t)(immediate << 20) >> 20;

    result.type = TYPE_S;


    switch (result.func3)
    {
        case 0x0:
            result.instruction_name = "SB";
            break;

        case 0x1:
            result.instruction_name = "SH";
            break;

        case 0x2:
            result.instruction_name = "SW";
            break;

        default:
            result.instruction_name = "UNKNOWN S";
            break;
    }

    return result;
}

decode_instruction decode_BType(uint32_t inp_inst)
{
    decode_instruction result;
    result.og_inp_inst = inp_inst;
    
    result.opcode = inp_inst & 0x7F;
    result.func3 = (inp_inst >> 12) & 0x07;
    result.rs1 = (inp_inst >> 15) & 0x1F;
    result.rs2 = (inp_inst >> 20) & 0x1F;
    result.rd = 0;
    result.func7 = 0;
    result.shamt = 0;


    uint32_t imm_12 =
        (inp_inst >> 31) & 0x1;
    uint32_t imm_10_5 =
        (inp_inst >> 25) & 0x3F;

    uint32_t imm_4_1 =
        (inp_inst >> 8) & 0x0F;

    uint32_t imm_11 =
        (inp_inst >> 7) & 0x1;


    uint32_t immediate =
        (imm_12 << 12) |
        (imm_11 << 11) |
        (imm_10_5 << 5) |
        (imm_4_1 << 1);


    result.immediate =
        (int32_t)(immediate << 19) >> 19;
    result.type = TYPE_B;

    switch (result.func3)
    {
        case 0x0:
            result.instruction_name = "BEQ";
            break;

        case 0x1:
            result.instruction_name = "BNE";
            break;

        case 0x4:
            result.instruction_name = "BLT";
            break;

        case 0x5:
            result.instruction_name = "BGE";
            break;

        case 0x6:
            result.instruction_name = "BLTU";
            break;

        case 0x7:
            result.instruction_name = "BGEU";
            break;

        default:
            result.instruction_name = "UNKNOWN B";
            break;
    }

    return result;
}

decode_instruction decode_UType(uint32_t inp_inst)
{
    decode_instruction result;

    result.og_inp_inst = inp_inst;
    result.opcode = inp_inst & 0x7F;
    
    result.rd = (inp_inst >> 7) & 0x1F;
    result.func3 = 0;
    result.func7 = 0;
    result.rs1 = 0;
    result.rs2 = 0;
    result.shamt = 0;

    result.immediate =
        (int32_t)(inp_inst & 0xFFFFF000);
    result.type = TYPE_U;


    if (result.opcode == 0x37)
    {
        result.instruction_name = "LUI";
    }
    else if (result.opcode == 0x17){
        result.instruction_name = "AUIPC";
    }
    else
    {
        result.instruction_name = "UNKNOWN U";    }

    return result;
}

decode_instruction decode_JType(uint32_t inp_inst)
{
    decode_instruction result;

    result.og_inp_inst = inp_inst;
    result.opcode = inp_inst & 0x7F;

    result.rd = (inp_inst >> 7) & 0x1F;
    result.func3 = 0;
    result.func7 = 0;
    result.rs1 = 0;
    result.rs2 = 0;
    result.shamt = 0;


    uint32_t imm_20 =
        (inp_inst >> 31) & 0x1;

    uint32_t imm_10_1 =
        (inp_inst >> 21) & 0x3FF;

    uint32_t imm_11 =
        (inp_inst >> 20) & 0x1;

    uint32_t imm_19_12 =
        (inp_inst >> 12) & 0xFF;


    uint32_t immediate =
        (imm_20 << 20) |
        (imm_19_12 << 12) |
        (imm_11 << 11) |
        (imm_10_1 << 1);

    result.immediate =
        (int32_t)(immediate << 11) >> 11;

    result.type = TYPE_J;
    result.instruction_name = "JAL";

    return result;
}