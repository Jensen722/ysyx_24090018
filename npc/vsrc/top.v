/*************************************************************************
	> File Name: top.v
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: Fri 07 Mar 2025 03:53:05 PM CST
 ************************************************************************/
`include "defines.v"

module top #(INST_ADDR_WIDTH = 32, REG_ADDR_WIDTH = 4 ,DATA_WIDTH = 32) (
  input clk,
  input rst
);

  /****************************  PC_Reg    *********************************/
  wire jump_flag;  
  wire [DATA_WIDTH-1 : 0] jump_addr;
  wire [INST_ADDR_WIDTH-1 : 0] pc;

  ysyx_24090018_pc_reg pc_reg (
    .clk(clk),
    .rst(rst),
    .jump_flag_i(jump_flag),
    .jump_addr_i(jump_addr),
    .pc_o(pc)
  );
  
  /****************************  IF state  **********************************/
  wire [DATA_WIDTH-1 : 0] inst;

  ysyx_24090018_IFU ifetch(
    .clk(clk),
    .pc_i(pc),
    .inst_o(inst)
  );

  /***************************** ID state  ************************************/
  wire [DATA_WIDTH-1 : 0] wdata_reg;
  wire Mem2Reg;
  wire MemRead;
  wire MemWrite;
  wire [2:0] MemOp;
  wire [1:0] ALUOp;
  wire pc_rs1_sel;
  wire alu_din1_sel1;
  wire alu_din1_sel2;
  wire alu_din2_sel1;
  wire alu_din2_sel2;
  wire cond_jump_ctrl;
  wire [2:0] cond_jump_type;
  wire no_cond_jump_ctrl;
  wire stop_flag;
  wire [DATA_WIDTH-1 : 0] imm;
  wire [DATA_WIDTH-1 : 0] reg_rdata1;
  wire [DATA_WIDTH-1 : 0] reg_rdata2;
  wire RegWrite;
  wire [REG_ADDR_WIDTH-1:0] rs1;
  wire [REG_ADDR_WIDTH-1:0] rs2;
  wire [REG_ADDR_WIDTH-1:0] rd;


  ysyx_24090018_IDU IDU (
    .inst_i(inst),
    .wdata_reg_i(wdata_reg),
    .Mem2Reg_o(Mem2Reg),
    .RegWrite_o(RegWrite),
    .MemRead_o(MemRead),
    .MemWrite_o(MemWrite),
    .MemOp_o(MemOp),
    .ALUOp_o(ALUOp),
    .pc_rs1_sel_o(pc_rs1_sel),
    .alu_din1_sel1_o(alu_din1_sel1),
    .alu_din1_sel2_o(alu_din1_sel2),
    .alu_din2_sel1_o(alu_din2_sel1),
    .alu_din2_sel2_o(alu_din2_sel2),
    .cond_jump_ctrl_o(cond_jump_ctrl),
    .cond_jump_type_o(cond_jump_type),
    .no_cond_jump_ctrl_o(no_cond_jump_ctrl),
    .stop_flag_o(stop_flag),
    .imm_o(imm),
    .rs1_o(rs1),
    .rs2_o(rs2),
    .rd_o(rd)
  );

  /***************************** EX stage  ***********************************/
  wire [DATA_WIDTH-1 : 0] alu_out;

  ysyx_24090018_EXU EXU ( 
    .inst_addr_i(pc),
    .inst_i(inst),
    .imm_i(imm),
    .reg_rdata1_i(reg_rdata1),
    .reg_rdata2_i(reg_rdata2),
    .ALUOp_i(ALUOp),
    .pc_rs1_sel_i(pc_rs1_sel),
    .alu_din1_sel1_i(alu_din1_sel1),
    .alu_din1_sel2_i(alu_din1_sel2),
    .alu_din2_sel1_i(alu_din2_sel1),
    .alu_din2_sel2_i(alu_din2_sel2),
    .cond_jump_ctrl_i(cond_jump_ctrl),
    .cond_jump_type_i(cond_jump_type),
    .no_cond_jump_ctrl_i(no_cond_jump_ctrl),
    .stop_flag_i(stop_flag),
    .jump_flag_o(jump_flag),
    .jump_addr_o(jump_addr),
    .alu_out_o(alu_out)
  );

  /***************************** Mem Access stage  ****************************/
  wire [DATA_WIDTH-1 : 0] mem_load_data; 
  
  ysyx_24090018_mem_access mem_access( 
    .mem_addr_i(alu_out),
    .mem_wdata_i(reg_rdata2),
    .MemRead_i(MemRead),
    .MemOp_i(MemOp),
    .MemWrite_i(MemWrite),
    .mem_load_data_o(mem_load_data)
  );

  /***************************** write back stage  *****************************/
  wire regwrite;
  ysyx_24090018_wb writeback( 
    .RegWrite_i(RegWrite),
    .mem_load_data_i(mem_load_data),
    .alu_out_i(alu_out),
    .Mem2Reg_i(Mem2Reg),
    .data_wb_o(wdata_reg),
    .RegWrite_o(regwrite)
  ); 


  //ysyx_24090018_RegisterFile module
  ysyx_24090018_RegisterFile rf(
    .clk(clk),
    .wdata_i(wdata_reg),
    .waddr_i(rd),
    .wen_i(regwrite),
    .rs1_i(rs1),
    .rs2_i(rs2),
    .rdata1_o(reg_rdata1),
    .rdata2_o(reg_rdata2)
  );
endmodule
