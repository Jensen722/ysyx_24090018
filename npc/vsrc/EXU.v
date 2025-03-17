/*************************************************************************
	> File Name: /home/jensen/ysyx-workbench/npc/vsrc/EXU.v
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: Fri 07 Mar 2025 10:57:54 AM CST
 ************************************************************************/
`include "defines.v"

module ysyx_24090018_EXU #(DATA_WIDTH = 32)(
  input [DATA_WIDTH-1 : 0] inst_addr_i,
  input [DATA_WIDTH-1 : 0] inst_i,
  input [DATA_WIDTH-1 : 0] imm_i,
  input [DATA_WIDTH-1 : 0] reg_rdata1_i,
  input [DATA_WIDTH-1 : 0] reg_rdata2_i,
  input [1:0] ALUOp_i,
  input pc_rs1_sel_i,
  input alu_din1_sel1_i,
  input alu_din1_sel2_i,
  input alu_din2_sel1_i,
  input alu_din2_sel2_i,
  input cond_jump_ctrl_i,
  input [2:0] cond_jump_type_i,
  input no_cond_jump_ctrl_i,
  input stop_flag_i,

  //to pc reg
  output jump_flag_o,
  output [DATA_WIDTH-1 : 0] jump_addr_o,

  //to mem and wb
  output [DATA_WIDTH-1 : 0] alu_out_o

);

  wire [DATA_WIDTH-1 : 0] alu_din1_muxout1;
  wire [DATA_WIDTH-1 : 0] alu_din1_muxout2;
  wire [DATA_WIDTH-1 : 0] alu_din2_muxout1;
  wire [DATA_WIDTH-1 : 0] alu_din2_muxout2;
  wire [3:0] alu_ctrl;
  wire [DATA_WIDTH-1 : 0] pc_or_rs1;
  wire cond_jump_flag;
  wire no_cond_jump_flag = no_cond_jump_ctrl_i;



export "DPI-C" function get_ebreak;
function get_ebreak;
  output bit stop_flag;
  stop_flag = stop_flag_i;  
endfunction

  //alu input multiplexer 
  ysyx_24090018_MuxKey #(2, 1, 32) alu_din1_mux1(
    .out(alu_din1_muxout1),
    .key(alu_din1_sel1_i),
    .lut({
      1'b0, reg_rdata1_i,
      1'b1, inst_addr_i    //pc
    })
  );
  ysyx_24090018_MuxKey #(2, 1, 32) alu_din1_mux2(
    .out(alu_din1_muxout2),
    .key(alu_din1_sel2_i),
    .lut({
      1'b0, alu_din1_muxout1,
      1'b1, 32'h0
    })
  );
  ysyx_24090018_MuxKey #(2, 1, 32) alu_din2_mux1(
    .out(alu_din2_muxout1),
    .key(alu_din2_sel1_i),
    .lut({
      1'b0, reg_rdata2_i,
      1'b1, imm_i
    })
  );
  ysyx_24090018_MuxKey #(2, 1, 32) alu_din2_mux2(
    .out(alu_din2_muxout2),
    .key(alu_din2_sel2_i),
    .lut({
      1'b0, alu_din2_muxout1,
      1'b1, 32'h4
    })
  );

  //alu control unit
  alu_ctrl alu_ctrl_i0(
    .ALUOp_i(ALUOp_i),
    .I30_i(inst_i[30]),
    .funct3_i(inst_i[14:12]), 
    .ALU_ctrl_o(alu_ctrl)
  );

  //alu module
  alu_32bit alu(
    .din1_i(alu_din1_muxout2),
    .din2_i(alu_din2_muxout2),
    .alu_sel_i(alu_ctrl),
    .cond_jump_type_i(cond_jump_type_i),
    .cond_jump_ctrl_i(cond_jump_ctrl_i),
    .cond_jump_flag_o(cond_jump_flag),
    .alu_out_o(alu_out_o)
  );

  //count jump addr
  ysyx_24090018_MuxKey #(2, 1, 32) pc_rs1_mux(
    .out(pc_or_rs1),
    .key(pc_rs1_sel_i),
    .lut({
      1'b0, inst_addr_i,  //pc
      1'b1, reg_rdata1_i  //rs1
    })
  );
  assign jump_addr_o = pc_or_rs1 + imm_i;  //综合成串行进位加法器
  assign jump_flag_o = no_cond_jump_flag || cond_jump_flag;

endmodule
