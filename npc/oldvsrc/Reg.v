/*************************************************************************
	> File Name: ysyx_24090018_RegisterFile.v
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2024年10月05日 星期六 19时59分47秒
 ************************************************************************/
module ysyx_24090018_Reg #(WIDTH = 1, RESET_VAL = 0) (
  input clk,
  input rst,
  input [WIDTH-1:0] din,
  output reg [WIDTH-1:0] dout,
  input wen
);
  always @(posedge clk) begin
    if (rst) dout <= RESET_VAL;
    else if (wen) dout <= din;
  end
endmodule
