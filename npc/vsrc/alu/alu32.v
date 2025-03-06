/*************************************************************************
	> File Name: alu.v
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2024年08月25日 星期日 16时50分16秒
 ************************************************************************/

module alu_32bit(
  input [31:0] din1_i,
  input [31:0] din2_i,
  input [3:0] alu_sel_i,
  output [31:0] alu_out_o
);

  wire [31:0] add_out;
  wire [31:0] sub_out;
  wire [31:0] and_out;
  wire [31:0] or_out;
  wire [31:0] xor_out;
  wire [31:0] sll_out;
  wire [31:0] srl_out;
  wire [31:0] sra_out;


  add_32bit add(
    .a(din1_i),
    .b(din2_i),
    .s(add_out)
  );

  sub_32bit sub(
    .a(din1_i),
    .b(din2_i),
    .s(sub_out)
  );

  and_32bit(
    .A(din1_i),
    .B(din2_i),
    .S(and_out)
  ); 

  or_32bit(
    .A(din1_i),
    .B(din2_i),
    .S(or_out)
  );
  
  xor_32bit(
    .A(din1_i),
    .B(din2_i),
    .S(xor_out)
  );

  sll sll_u1(
    .din(din1_i),
    .shamt(din2_i[4:0]),
    .dout(sll_out)
  ); 

  srl srl_u1(
    .din(din1_i),
    .shamt(din2_i[4:0]),
    .dout(srl_out)
  ); 

  sra sra_u1(
    .din(din1_i),
    .shamt(din2_i[4:0]),
    .dout(sra_out)
  ); 

  //mux 16 to 1
  ysyx_24090018_MuxKeyWithDefault #(8, 4, 32) i0 (
    .out(alu_out_o),
    .key(alu_sel_i),
    .default_out(32'b0),
    .lut({
      4'b0000, and_out,
      4'b0001, or_out,
      4'b0010, add_out,
      4'b0011, xor_out,
      4'b0100, sll_out,
      4'b0101, srl_out,
      4'b0110, sub_out,
      4'b0111, sra_out
    })
  );



endmodule
