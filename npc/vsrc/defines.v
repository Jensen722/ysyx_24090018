/*************************************************************************
	> File Name: Defines.v
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2025年03月05日 星期三 14时11分37秒
 ************************************************************************/

//PC
`define ysyx_24090018_PC_BaseAddr 32'h8000_0000

//Enable & Disable Signal
`define ysyx_24090018_RstEnable 1'b0
`define ysyx_24090018_WriteEnable 1
`define ysyx_24090018_WriteDisable 0

//Zero
`define ysyx_24090018_RegZero 0
`define ysyx_24090018_ZeroWord 32'h0000_0000


//U Type 0110111
`define ysyx_24090018_TYPE_U_0110111         7'b0110111         //lui

//U Type 0010111
`define ysyx_24090018_TYPE_U_0010111         7'b0010111         //auipc

//J Type 1101111
`define ysyx_24090018_TYPE_J_0110111         7'b1101111         //jal

//I Type 0010011 
`define ysyx_24090018_TYPE_I_0010011         7'b0010011
`define ysyx_24090018_INST_ADDI              3'b000             //addi
`define ysyx_24090018_INST_SLLI              3'b001             //slli
`define ysyx_24090018_INST_SLTI              3'b010             //slti
`define ysyx_24090018_INST_SLTIU             3'b011             //sltiu
`define ysyx_24090018_INST_XORI              3'b100             //xori
`define ysyx_24090018_INST_SRI               3'b101             //sri
`define ysyx_24090018_INST_ORI               3'b110             //ori
`define ysyx_24090018_INST_ANDI              3'b111             //andi

//I Type 0000011
`define ysyx_24090018_TYPE_I_0000011         7'b0000011
`define ysyx_24090018_INST_LB                3'b000             //lb
`define ysyx_24090018_INST_LH                3'b001             //lh
`define ysyx_24090018_INST_LW                3'b010             //lw
`define ysyx_24090018_INST_LBU               3'b100             //lbu
`define ysyx_24090018_INST_LHU               3'b101             //lhu

//I Type 1100111
`define ysyx_24090018_TYPE_I_1100111         7'b1100111         //jalr

//I Type 0001111
`define ysyx_24090018_TYPE_I_0001111         7'b0001111     
`define ysyx_24090018_INST_FENCE             3'b000             //fence
`define ysyx_24090018_INST_FENCE.I           3'b001             //fence.i

//I Type 1110011
`define ysyx_24090018_TYPE_I_1110011         7'b1110011
`define ysyx_24090018_INST_EBREAK_ECALL      3'b000             //ebreak & ecall
`define ysyx_24090018_INST_CSRRW             3'b001             //csrrw
`define ysyx_24090018_INST_CSRRS             3'b010             //csrrs
`define ysyx_24090018_INST_CSRRC             3'b011             //csrrc
`define ysyx_24090018_INST_CSRRWI            3'b101             //csrrwi
`define ysyx_24090018_INST_CSRRSI            3'b110             //csrrsi
`define ysyx_24090018_INST_CSRRCI            3'b111             //csrrci

//R Type 0110011
`define ysyx_24090018_TYPE_R_0110011         7'b0110011
`define ysyx_24090018_INST_ADD_SUB           3'b000             //add & sub
`define ysyx_24090018_INST_SLL               3'b001             //sll
`define ysyx_24090018_INST_SLT               3'b010             //slt
`define ysyx_24090018_INST_SLTU              3'b011             //sltu
`define ysyx_24090018_INST_XOR               3'b100             //xori
`define ysyx_24090018_INST_SRL_SRA           3'b101             //srl & sra
`define ysyx_24090018_INST_OR                3'b110             //or
`define ysyx_24090018_INST_AND               3'b111             //and

//S Type 0100011
`define ysyx_24090018_TYPE_S_0100011         7'b0100011
`define ysyx_24090018_INST_SB                3'b001             //sb
`define ysyx_24090018_INST_SH                3'b001             //sh
`define ysyx_24090018_INST_SW                3'b001             //sw


//B Type 1100011
`define ysyx_24090018_TYPE_B_1100011         7'b1100011
`define ysyx_24090018_INST_BEQ               3'b000             //beq
`define ysyx_24090018_INST_BNE               3'b001             //bne
`define ysyx_24090018_INST_BLT               3'b100             //blt
`define ysyx_24090018_INST_BGE               3'b101             //bge
`define ysyx_24090018_INST_BLTU              3'b110             //bltu
`define ysyx_24090018_INST_BGEU              3'b111             //bgeu


