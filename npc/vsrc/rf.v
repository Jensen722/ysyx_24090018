/*************************************************************************
	> File Name: rf.v
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2025年03月05日 星期三 15时08分23秒
 ************************************************************************/

module ysyx_24090018_RegisterFile #(REG_ADDR_WIDTH = 4, DATA_WIDTH = 32) (
  input clk,
  input [DATA_WIDTH-1:0] wdata_i,
  input [REG_ADDR_WIDTH-1:0] waddr_i,
  input wen_i,

  input [REG_ADDR_WIDTH-1:0] rs1_i,
  input [REG_ADDR_WIDTH-1:0] rs2_i,
  output [DATA_WIDTH-1:0] rdata1_o,
  output [DATA_WIDTH-1:0] rdata2_o
);

  reg [DATA_WIDTH-1:0] rf [2**REG_ADDR_WIDTH-1:0];  //寄存器数量
  always @(posedge clk) begin //0号寄存器永远存0
    rf[0] <= {DATA_WIDTH{1'b0}};
  end

  always @(posedge clk) begin
    if (wen_i && waddr_i != 0) begin
      rf[waddr_i] <= wdata_i;
    end
  end

  assign rdata1_o = rf[rs1_i];  //read src1 data
  assign rdata2_o = rf[rs2_i];  //read src2 data

endmodule
