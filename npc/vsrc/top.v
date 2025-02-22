/*************************************************************************
	> File Name: top.v
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2024年10月07日 星期一 14时28分00秒
 ************************************************************************/
module top #(PC_ADDR = 32, REG_ADDR = 5, DATA_WIDTH = 32)(
  input clk,
  input rst,
  input [DATA_WIDTH-1:0] inst_i,
  output [PC_ADDR-1:0] pc_o,
  output [DATA_WIDTH-1:0] op1_o,
  output [DATA_WIDTH-1:0] op2_o,
  output [DATA_WIDTH-1:0] rf_wdata_o

);
  wire [DATA_WIDTH-1:0] rf_rdata1;
  wire [DATA_WIDTH-1:0] rf_rdata2;
  wire [REG_ADDR-1:0] rf_raddr1;
  wire [REG_ADDR-1:0] rf_raddr2;
  wire [REG_ADDR-1:0] rf_waddr;
  wire rf_we;
  wire jump_flag;
  wire [DATA_WIDTH-1:0] jump_addr;
  wire [DATA_WIDTH-1:0] op1_jump_o;
  wire [DATA_WIDTH-1:0] op2_jump_o;

  //PC_Reg
  ysyx_24090018_pc_reg pc_reg(
    .clk(clk),
    .rst(rst),
    .jump_flag(jump_flag),
    .jump_addr(jump_addr),
    .pc_o(pc_o)
  );
 /* //IFU
  ysyx_24090018_IFU IFU(
    .clk(clk),
    .pc(pc_o),
    .inst_o(inst_i)
  );*/

  //IDU
  ysyx_24090018_IDU IDU(
    .inst_i(inst_i),
    .inst_addr_i(pc_o),
    .rf_rdata1_i(rf_rdata1),
    .rf_rdata2_i(rf_rdata2),
    .rf_raddr1_o(rf_raddr1),
    .rf_raddr2_o(rf_raddr2),
    .rf_waddr_o(rf_waddr),
    .rf_we_o(rf_we),
    .op1_jump_o(op1_jump_o),
    .op2_jump_o(op2_jump_o),
    .jump_flag_o(jump_flag),
    .op1_o(op1_o),
    .op2_o(op2_o)
  );

  //EXU
  ysyx_24090018_EXU EXU(
    .op1_i(op1_o),
    .op2_i(op2_o),
    .inst_i(inst_i),
    .inst_addr_i(pc_o),
    .op1_jump_i(op1_jump_o),
    .op2_jump_i(op2_jump_o),
    .rf_wdata_o(rf_wdata_o),
    .jump_addr_o(jump_addr)
  ); 

  //RegisterFile
  ysyx_24090018_RegisterFile RF(
    .clk(clk),
    .wdata(rf_wdata_o),
    .waddr(rf_waddr),
    .wen(rf_we),
    .rs1(rf_raddr1),
    .rs2(rf_raddr2),
    .rdata1(rf_rdata1),
    .rdata2(rf_rdata2)
      );
endmodule
