/*************************************************************************
	> File Name: alu_ctrl.v
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2024年12月05日 星期四 16时52分37秒
 ************************************************************************/
module alu_ctrl(
  input [1:0] ALUOp_i_i,
  input I30_i,
  input I14_i,
  input I13_i,
  input I12_i,

  output [3:0] ALU_ctrl_o
);

  assign ALU_ctrl_o[3] = 0;
  assign ALU_ctrl_o[2] = ALUOp_i_i[0] || ALUOp_i[1] && !I13_i && (!I14_i && (I30_i ^ I12_i) || I14_i && I12_i);
  assign ALU_ctrl_o[1] = !ALUOp_i_i[1] && !ALUOp_i[0] || ALUOp_i[0] || (ALUOp_i[1] && !I13_i && (!I14_i && !I12_i || I14_i && !(I30_i ^ I12_i)));
  assign ALU_ctrl_o[0] = ALUOp_i_i[1] && I14_i && (!I30_i && !I12_i || !I13_i && I12_i);

endmodule
