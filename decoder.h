#ifndef DECODER_HEADER
#define DECODER_HEADER

#include <stdint.h>

typedef enum
{
    TYPE_UNKNOWN,
    TYPE_R,
    TYPE_I,
    TYPE_S,
    TYPE_B,
    TYPE_U,
    TYPE_J
} InstructionType;


typedef struct
{
    uint32_t og_inp_inst;

    InstructionType type;

    int32_t immediate;

    uint8_t opcode;
    uint8_t func3;
    uint8_t func7;

    uint8_t rd;
    uint8_t rs1;
    uint8_t rs2;
    uint8_t shamt; // shift amount for srli,srai and slli

    const char *instruction_name; // This function helps to identify in depth type of instruction like ADD,SUB,SRL for RType inst's

} decode_instruction;



decode_instruction decoder(uint32_t inp_inst); // This is the main decoder that helps to extract opcode



decode_instruction decode_RType(uint32_t inp_inst); //These decoders are used to perform Type specific tasks
decode_instruction decode_IType(uint32_t inp_inst);
decode_instruction decode_SType(uint32_t inp_inst);
decode_instruction decode_BType(uint32_t inp_inst);
decode_instruction decode_UType(uint32_t inp_inst);
decode_instruction decode_JType(uint32_t inp_inst);


const char *type_name(InstructionType type); // This is a helper function for identifying type name of specific inst

#endif