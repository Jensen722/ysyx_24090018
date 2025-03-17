/*************************************************************************
	> File Name: ImmGen.v
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2025年03月05日 星期三 15时24分12秒
 ************************************************************************/
`include "defines.v"

module Imm_Gen(
  input [31:0] inst_i,
  output [31:0] imm_o
);

  wire [6:0] opcode = inst_i[6:0];

  wire [31:0] imm_I = {{20{inst_i[31]}}, inst_i[31:20]};
  wire [31:0] imm_U = {inst_i[31:12], {12{1'b0}}};
  wire [31:0] imm_J = {{12{inst_i[31]}}, inst_i[19:12], inst_i[20], inst_i[30:21], 1'b0};
  wire [31:0] imm_B = {{20{inst_i[31]}}, inst_i[7], inst_i[30:25], inst_i[11:8], 1'b0};
  wire [31:0] imm_S = {{20{inst_i[31]}}, inst_i[31:25], inst_i[11:7]};

  //imm select
  ysyx_24090018_MuxKeyWithDefault #(7, 7, 32) i0 (
    .out(imm_o),
    .key(opcode),
    .default_out(32'b0),
    .lut({
      `ysyx_24090018_TYPE_I_0010011, imm_I,  //jalr addi slti ... andi
      `ysyx_24090018_TYPE_I_0000011, imm_I,  //lb lh lw lbu lhu
      `ysyx_24090018_TYPE_U_0110111, imm_U,  //lui
      `ysyx_24090018_TYPE_U_0010111, imm_U,  //auipc
      `ysyx_24090018_TYPE_B_1100011, imm_B,  //beq bne bge bltu bgeu
      `ysyx_24090018_TYPE_J_1101111, imm_J,  //jal
      `ysyx_24090018_TYPE_S_0100011, imm_S   //sb sh sw
    })
  );
endmodule
