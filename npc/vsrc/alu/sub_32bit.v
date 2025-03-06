/*************************************************************************
	> File Name: sub_32bit.v
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2024年12月06日 星期五 19时24分39秒
 ************************************************************************/
module sub_32bit(
  input [31:0] a,
  input [31:0] b,
  output [31:0] s
);

  wire [31:0] b_complement;
  wire carry;
  assign b_complement = ~b + 1;
  add_32bit add(
    .a(a),
    .b(b_complement),
    .s(s)
  );
endmodule
