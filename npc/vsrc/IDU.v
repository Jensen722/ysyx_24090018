/*************************************************************************
	> File Name: /home/jensen/ysyx-workbench/npc/vsrc/IDU.v
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: Thu 06 Mar 2025 07:16:29 PM CST
 ************************************************************************/
`include "defines.v"

module ysyx_24090018_IDU #(DATA_WIDTH = 32, REG_ADDR_WIDTH = 4) (
  //from inst memory
  input [31:0] inst_i,

  //from write back module
  input [DATA_WIDTH-1 : 0] wdata_reg_i,

  //to mem
  output  Mem2Reg_o,
  output  MemRead_o,
  output  MemWrite_o,
  output [2:0] MemOp_o,
  output  RegWrite_o,

  //to exu
  output  [1:0] ALUOp_o,
  output  pc_rs1_sel_o,
  output  alu_din1_sel1_o,     //1 is for pc and 0 is for rs1
  output  alu_din1_sel2_o,     //1 is for 0 and 0 is for rs1 or pc
  output  alu_din2_sel1_o,     //0 is for rs2 and 1 is for imm
  output  alu_din2_sel2_o,     //0 is for rs2 or imm and 1 is for 4'h4
  output  cond_jump_ctrl_o,
  output [2:0] cond_jump_type_o,
  output  no_cond_jump_ctrl_o,  //jal jalr
  output  stop_flag_o,          //ebreak

  output [DATA_WIDTH-1 : 0] imm_o,

  //to rf
  output [REG_ADDR_WIDTH-1 : 0] rs1_o,
  output [REG_ADDR_WIDTH-1 : 0] rs2_o,
  output [REG_ADDR_WIDTH-1 : 0] rd_o

);
  
  assign  rs1_o = inst_i[18:15];
  assign  rs2_o = inst_i[23:20];
  assign  rd_o  = inst_i[10:7];

export "DPI-C" function get_inst;
function get_inst;
  output  [31:0] inst;
  inst = inst_i;
endfunction

  //ysyx_24090018_ctrl module 
  ysyx_24090018_ctrl ctrl(
    .inst_i(inst_i),
    .ALUOp_o(ALUOp_o),
    .Mem2Reg_o(Mem2Reg_o),
    .RegWrite_o(RegWrite_o),
    .MemRead_o(MemRead_o),
    .MemWrite_o(MemWrite_o),
    .MemOp_o(MemOp_o),
    .pc_rs1_sel_o(pc_rs1_sel_o),
    .alu_din1_sel1_o(alu_din1_sel1_o),
    .alu_din1_sel2_o(alu_din1_sel2_o),
    .alu_din2_sel1_o(alu_din2_sel1_o),
    .alu_din2_sel2_o(alu_din2_sel2_o),
    .cond_jump_ctrl_o(cond_jump_ctrl_o),
    .cond_jump_type_o(cond_jump_type_o),
    .no_cond_jump_ctrl_o(no_cond_jump_ctrl_o),
    .stop_flag_o(stop_flag_o)
  );

  //Imm_Gen module 
  Imm_Gen ImmGen (
    .inst_i(inst_i),
    .imm_o(imm_o)
  );

  //ysyx_24090018_RegisterFile module
  /*
  ysyx_24090018_RegisterFile rf(
    .clk(clk),
    .wdata_i(wdata_reg_i),
    .waddr_i(rd),
    .wen_i(RegWrite),
    .rs1_i(rs1),
    .rs2_i(rs2),
    .rdata1_o(reg_rdata1_o),
    .rdata2_o(reg_rdata2_o)
  );*/

  endmodule
