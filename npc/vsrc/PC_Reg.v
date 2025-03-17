/*************************************************************************
	> File Name: PC_Reg.v
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2025年03月05日 星期三 15时05分24秒
 ************************************************************************/
`include "defines.v"

module ysyx_24090018_pc_reg #(ADDR_WIDTH = 32) (
  input                         clk,
  input                         rst,
  input                         jump_flag_i,
  input      [31:0]             jump_addr_i,
  output reg [ADDR_WIDTH-1 : 0] pc_o
);


initial begin
  pc_o = 32'h80000000;
end

  wire [31:0] pc_muxout;

  ysyx_24090018_MuxKey #(2, 1, 32) i0(
    .out(pc_muxout),
    .key(jump_flag_i),
    .lut({
      1'b0, pc_o + 32'h4,
      1'b1, jump_addr_i
    })
  );

  always @(posedge clk) begin
    if(rst == `ysyx_24090018_RstEnable) begin
      pc_o <= `ysyx_24090018_PC_BaseAddr;
    end
    else begin
      pc_o <= pc_muxout;
    end
  end
endmodule
