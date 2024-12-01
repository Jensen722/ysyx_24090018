/*************************************************************************
	> File Name: Defines.v
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2024年10月06日 星期日 15时34分48秒
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


//I Type 
`define ysyx_24090018_INST_I_0010011 7'b0010011
`define ysyx_24090018_INST_I_1110011 7'b1110011
`define ysyx_24090018_funct3_ADDI 3'b000
`define ysyx_24090018_funct3_EBREAK_ECALL 3'b000



