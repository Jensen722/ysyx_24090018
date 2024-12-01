/*************************************************************************
	> File Name: PC_Reg.v
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2024年10月06日 星期日 11时57分56秒
 ************************************************************************/
`include "Defines.v"

module ysyx_24090018_pc_reg #(ADDR_WIDTH = 32) (
  input clk,
  input rst,
  output reg [ADDR_WIDTH-1 : 0] pc_o
);
  always @(posedge clk) begin
    if(rst == `ysyx_24090018_RstEnable) begin
      pc_o <= `ysyx_24090018_PC_BaseAddr;
    end
    else begin
      pc_o <= pc_o + 32'h4;
    end
  end
endmodule
