/*************************************************************************
	> File Name: sra.v
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2024年12月05日 星期四 19时21分27秒
 ************************************************************************/
module sra(
  input [31:0] din,
  input [4:0] shamt,
  output [31:0] dout
);
  assign dout = din >>> shamt; 
endmodule
