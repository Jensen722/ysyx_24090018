/*************************************************************************
	> File Name: alu_ctrl.v
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2024年12月05日 星期四 16时52分37秒
 ************************************************************************/
module alu_ctrl(
  input [1:0] ALUOp_i,
  input I30_i,
  input [2:0] funct3_i,

  output reg [3:0] ALU_ctrl_o
);
/*
  assign ALU_ctrl_o[3] = 0;
  assign ALU_ctrl_o[= ALUOp_i[0] || ALUOp_i[1] && !I13_i && (!I14_i && (I30_i ^ I12_i) || I14_i && I12_i);
  assign ALU_ctrl_o[1] = !ALUOp_i[1] && !ALUOp_i[0] || ALUOp_i[0] || (ALUOp_i[1] && !I13_i && (!I14_i && !I12_i || I14_i && !(I30_i ^ I12_i)));
  assign ALU_ctrl_o[0] = ALUOp_i[1] && I14_i && (!I30_i && !I12_i || !I13_i && I12_i);
*/

wire [3:0] alu_op_Itype;
wire [3:0] alu_op_Rtype;
wire [3:0] alu_op_Btype;
  ysyx_24090018_MuxKey #(8, 3, 4) alu_op1 (
    .out(alu_op_Itype),
    .key(funct3_i),
    .lut({
      3'b000, 4'b0010,
      3'b001, 4'b0100,
      3'b010, 4'b1001,
      3'b011, 4'b1000,
      3'b100, 4'b0011,
      3'b101, I30_i ? 4'b0111 : 4'b0101,
      3'b110, 4'b0001,
      3'b111, 4'b0000
    })
  );

  ysyx_24090018_MuxKey #(8, 3, 4) alu_op2 (
    .out(alu_op_Rtype),
    .key(funct3_i),
    .lut({
      3'b000, I30_i ? 4'b0110 : 4'b0010,
      3'b001, 4'b0100,
      3'b010, 4'b1001,
      3'b011, 4'b1000,
      3'b100, 4'b0011,
      3'b101, I30_i ? 4'b0111 : 4'b0101,
      3'b110, 4'b0001,
      3'b111, 4'b0000
    })
  );

  ysyx_24090018_MuxKey #(4, 2, 4) alu_ctrl_sel (
    .out(ALU_ctrl_o),
    .key(ALUOp_i),
    .lut({
      2'b00, 4'b0010,
      2'b01, 4'b0110,
      2'b10, alu_op_Itype,
      2'b11, alu_op_Rtype
    })
  );

endmodule
