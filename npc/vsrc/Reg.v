/*************************************************************************
	> File Name: ysyx_24090018_RegisterFile.v
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2024年10月05日 星期六 19时59分47秒
 ************************************************************************/
module ysyx_24090018_Reg #(WIDTH = 32, RESET_VAL = 0) (
  input                     clk,
  input                     rst,
  input                     wen_i,
  input       [WIDTH-1:0]   din_i,
  output reg  [WIDTH-1:0]   dout_o
);
  always @(posedge clk) begin
    if (rst) begin
      dout_o <= RESET_VAL;
    end 
    else  begin
      if (wen_i) dout_o <= din_i;
    end
  end
endmodule
