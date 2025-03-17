/*************************************************************************
	> File Name: /home/jensen/ysyx-workbench/npc/vsrc/ctrl.v
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: Thu 06 Mar 2025 04:09:51 PM CST
 ************************************************************************/
`include "defines.v"

module ysyx_24090018_ctrl #(INST_WIDTH = 32)(
  input [INST_WIDTH - 1 : 0] inst_i,
  output  [1:0] ALUOp_o,
  output  Mem2Reg_o,
  output  RegWrite_o,
  output  MemRead_o,
  output  MemWrite_o,
  output [2:0] MemOp_o,
  output  pc_rs1_sel_o,
  output  alu_din1_sel1_o,     //1 is for pc and 0 is for rs1
  output  alu_din1_sel2_o,     //1 is for 0 and 0 is for rs1 or pc
  output  alu_din2_sel1_o,     //0 is for rs2 and 1 is for imm
  output  alu_din2_sel2_o,     //0 is for rs2 or imm and 1 is for 4'h4
  output  cond_jump_ctrl_o,
  output [2:0] cond_jump_type_o,
  output  no_cond_jump_ctrl_o, //jal jalr
  output  stop_flag_o          //ebreak
);

  wire [6:0] opcode = inst_i[6:0];
  wire [2:0] funct3 = inst_i[14:12];

  //ALUOp_o select logic
  ysyx_24090018_MuxKeyWithDefault #(3, 7, 2) aluop (
    .out(ALUOp_o),
    .key(opcode),
    .default_out(2'b00),
    .lut({
      `ysyx_24090018_TYPE_B_1100011, 2'b01,      //bne beq ... blt
      `ysyx_24090018_TYPE_I_0010011, 2'b10,      //addi slli slti ... andi
      `ysyx_24090018_TYPE_R_0110011, 2'b11       //add or ... and
    })
  );

  //Mem2Reg_o select logic
  ysyx_24090018_MuxKeyWithDefault #(1, 7, 1) mem2reg (
    .out(Mem2Reg_o),
    .key(opcode),
    .default_out(1'b0),
    .lut({
      `ysyx_24090018_TYPE_I_0000011, 1'b1
    })
  );

  //RegWrite_o select logic
  ysyx_24090018_MuxKeyWithDefault #(7, 7, 1) regwrite(
    .out(RegWrite_o),
    .key(opcode),
    .default_out(1'b0),
    .lut({
      `ysyx_24090018_TYPE_I_0010011, 1'b1,     //addi ... andi
      `ysyx_24090018_TYPE_I_1100111, 1'b1,     //jarl
      `ysyx_24090018_TYPE_J_1101111, 1'b1,     //jal
      `ysyx_24090018_TYPE_U_0010111, 1'b1,     //auipc
      `ysyx_24090018_TYPE_U_0110111, 1'b1,     //lui
      `ysyx_24090018_TYPE_R_0110011, 1'b1,     //add ... and
      `ysyx_24090018_TYPE_I_0000011, 1'b1      //lb ... lw   
    })
  );

  //MemRead_o select logic
  ysyx_24090018_MuxKeyWithDefault #(1, 7, 1) memread (
    .out(MemRead_o),
    .key(opcode),
    .default_out(1'b0),
    .lut({
      `ysyx_24090018_TYPE_I_0000011, 1'b1
    })
  );

  //MemWrite_o select logic
  ysyx_24090018_MuxKeyWithDefault #(1, 7, 1) memwrite (
    .out(MemWrite_o),
    .key(opcode),
    .default_out(1'b0),
    .lut({
      `ysyx_24090018_TYPE_S_0100011, 1'b1
    })
  );

  //select memory opreation according to funct3
  assign MemOp_o = funct3;

  //pc_rs1_sel -> 1 : rs1 , 0 : pc
  ysyx_24090018_MuxKeyWithDefault #(2, 7, 1) pc_rs1_sel (
    .out(pc_rs1_sel_o),
    .key(opcode),
    .default_out(1'b0),
    .lut({
      `ysyx_24090018_TYPE_I_1100111, 1'b1,      //jarl
      `ysyx_24090018_TYPE_J_1101111, 1'b0       //jal
    })
  );
  //alu_din1_sel1_o select logic
  ysyx_24090018_MuxKeyWithDefault #(3, 7, 1) alu_din1_sel1 (
    .out(alu_din1_sel1_o),
    .key(opcode),
    .default_out(1'b0),
    .lut({
      `ysyx_24090018_TYPE_I_1100111, 1'b1,      //jarl
      `ysyx_24090018_TYPE_J_1101111, 1'b1,      //jal
      `ysyx_24090018_TYPE_U_0010111, 1'b1       //auipc
    })
  );

  //alu_din1_sel2_o select logic
  ysyx_24090018_MuxKeyWithDefault #(1, 7, 1) alu_din1_sel2 (
    .out(alu_din1_sel2_o),
    .key(opcode),
    .default_out(1'b0),
    .lut({
      `ysyx_24090018_TYPE_U_0110111, 1'b1      //lui
    })
  );

 //alu_din2_sel1_o select logic -> 0 is for rs2 and 1 is for imm
  ysyx_24090018_MuxKeyWithDefault #(7, 7, 1) alu_din2_sel1 (
    .out(alu_din2_sel1_o),
    .key(opcode),
    .default_out(1'b0),
    .lut({
      `ysyx_24090018_TYPE_I_0000011, 1'b1,      //lb, lh ... lhu
      `ysyx_24090018_TYPE_I_0010011, 1'b1,      //addi ... andi
      `ysyx_24090018_TYPE_I_1100111, 1'b1,      //jarl
      `ysyx_24090018_TYPE_J_1101111, 1'b1,      //jal
      `ysyx_24090018_TYPE_U_0010111, 1'b1,      //auipc
      `ysyx_24090018_TYPE_U_0110111, 1'b1,      //lui
      `ysyx_24090018_TYPE_S_0100011, 1'b1       //sb ... sw
    })                                 
  );

  //alu_din2_sel2_o select logic -> 0 is for rs2 or imm and 1 is for 4'h4 ->
  //pc + 4 -> rd
  ysyx_24090018_MuxKeyWithDefault #(2, 7, 1) alu_din2_sel2 (
    .out(alu_din2_sel2_o),
    .key(opcode),
    .default_out(1'b0),
    .lut({
      `ysyx_24090018_TYPE_I_1100111, 1'b1,      //jarl
      `ysyx_24090018_TYPE_J_1101111, 1'b1       //jal
    })
  );

  assign cond_jump_type_o = funct3;
  //cond_jump_ctrl_o
  ysyx_24090018_MuxKeyWithDefault #(1, 7, 1) cond_jump_ctrl (
    .out(cond_jump_ctrl_o),
    .key(opcode),
    .default_out(1'b0),
    .lut({
      `ysyx_24090018_TYPE_B_1100011, 1'b1      //bne ... bge
    })
  );

  //no_cond_jump_ctrl_o
  ysyx_24090018_MuxKeyWithDefault #(2, 7, 1) no_cond_jump_ctrl (
    .out(no_cond_jump_ctrl_o),
    .key(opcode),
    .default_out(1'b0),
    .lut({
      `ysyx_24090018_TYPE_I_1100111, 1'b1,      //jarl
      `ysyx_24090018_TYPE_J_1101111, 1'b1       //jal
    })
  );

  //stop flag
  ysyx_24090018_MuxKeyWithDefault #(1, 7, 1) inst_ebreak (
    .out(stop_flag_o),
    .key(opcode),
    .default_out(1'b0),
    .lut({
      `ysyx_24090018_TYPE_I_1110011, inst_i[20] ? 1'b1 : 1'b0      //ebreak     
    })
  );


endmodule
