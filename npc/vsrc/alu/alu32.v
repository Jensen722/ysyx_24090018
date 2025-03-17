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
  input [2:0] cond_jump_type_i,
  input cond_jump_ctrl_i,

  output cond_jump_flag_o,
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
  wire [31:0] sltiu_sltu_out;
  wire [31:0] slti_slt_out;


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

  and_32bit and_32bit_i0(
    .A(din1_i),
    .B(din2_i),
    .S(and_out)
  ); 

  or_32bit or_32bit_i0(
    .A(din1_i),
    .B(din2_i),
    .S(or_out)
  );
  
  xor_32bit xor_32bit_i0(
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

  assign sltiu_sltu_out = ltu_flag ? 32'h1 : 32'h0;
  assign slti_slt_out = lt_flag ? 32'h1 : 32'h0;

  //有符号数比较
  reg lt_flag;
  wire ge_flag = !lt_flag;
  always @(*) begin
    if(din1_i[31] == din2_i[31]) begin
      lt_flag = ($signed(sub_out) < 0) ? 1'b1 : 1'b0;
    end
    else begin
      lt_flag = (din1_i[31] == 1'b1) ? 1'b1 : 1'b0;
    end
  end

  //无符号数比较
  /*wire ltu_flag;
  wire geu_flag;
  assign ltu_flag = sub_out[31];// == 1'b1 ? 1'b1 : 1'b0;//($signed(sub_out) < 0);
  assign geu_flag = !ltu_flag;*/
  reg ltu_flag;
  wire geu_flag = !ltu_flag;
  always @(*) begin
    if(din1_i[31] == din2_i[31]) begin
      ltu_flag = ($signed(sub_out) < 0) ? 1'b1 : 1'b0;
    end
    else begin
      ltu_flag = (din1_i[31] == 1'b1) ? 1'b0 : 1'b1;
    end
  end


  wire cond_compare_out;
  //jump flag logic
  ysyx_24090018_MuxKeyWithDefault #(6, 3, 1) cond_jump_type(
    .out(cond_compare_out),
    .key(cond_jump_type_i),
    .default_out(1'b0),
    .lut({
      3'b000, (alu_out_o == 0) ? 1'b1 : 1'b0,
      3'b001, (alu_out_o != 0) ? 1'b1 : 1'b0,
      3'b100, lt_flag,
      3'b101, ge_flag,
      3'b110, ltu_flag,
      3'b111, geu_flag
    })
  );
  assign cond_jump_flag_o = cond_jump_ctrl_i && cond_compare_out;

  //mux 16 to 1
  ysyx_24090018_MuxKeyWithDefault #(10, 4, 32) i0 (
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
      4'b0111, sra_out,
      4'b1000, sltiu_sltu_out,
      4'b1001, slti_slt_out
    })
  );



endmodule
