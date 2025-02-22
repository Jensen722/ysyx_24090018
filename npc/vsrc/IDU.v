/*************************************************************************
	> File Name: IDU.v
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2024年10月06日 星期日 16时14分47秒
 ************************************************************************/
`include "Defines.v"

module ysyx_24090018_IDU #(DATA_WIDTH = 32, ADDR_WIDTH = 5) (
  //from IFU
  input [DATA_WIDTH-1 : 0] inst_i,
  input [DATA_WIDTH-1 : 0] inst_addr_i,

  //from RegisterFile
  input [DATA_WIDTH-1 : 0] rf_rdata1_i,
  input [DATA_WIDTH-1 : 0] rf_rdata2_i,
  //to RegisterFile
  output reg [ADDR_WIDTH-1 : 0] rf_raddr1_o,
  output reg [ADDR_WIDTH-1 : 0] rf_raddr2_o,
  output reg [ADDR_WIDTH-1 : 0] rf_waddr_o,
  output reg rf_we_o,

  //to EXU
  output reg [DATA_WIDTH-1 : 0] op1_jump_o,
  output reg [DATA_WIDTH-1 : 0] op2_jump_o,
  output reg jump_flag_o,
  output reg [DATA_WIDTH-1 : 0] op1_o,
  output reg [DATA_WIDTH-1 : 0] op2_o
);
  wire [6:0] opcode = inst_i[6:0];
  wire [2:0] funct3 = inst_i[14:12];
  wire [6:0] funct7 = inst_i[31:25];
  wire [4:0] rd = inst_i[11:7];
  wire [4:0] rs1 = inst_i[19:15];
  wire [4:0] rs2 = inst_i[24:20];


  always @(*) begin
    case(opcode) 
        `ysyx_24090018_INST_I_0010011: begin //addi & slti & sltiu & xori & ori
        //& andi & slli & srli & srai 
            op1_o = rf_rdata1_i;
            op2_o = {{20{inst_i[31]}}, inst_i[31:20]};
            rf_we_o = `ysyx_24090018_WriteEnable;
            jump_flag_o = 1'b0;
          op1_jump_o = `ysyx_24090018_ZeroWord;
          op2_jump_o = `ysyx_24090018_ZeroWord;
            rf_raddr1_o = rs1;
            rf_raddr2_o = `ysyx_24090018_RegZero;
            rf_waddr_o = rd;
        end
        `ysyx_24090018_INST_I_1110011: begin  //ebreak & ecall & csrrw & csrrs
        //& csrrc & csrrwi &csrrsi & csrrci
            op1_o = rf_rdata1_i;
            op2_o = {{20{inst_i[31]}}, inst_i[31:20]};
            rf_we_o = `ysyx_24090018_WriteEnable;
            rf_raddr1_o = rs1;
            rf_raddr2_o = `ysyx_24090018_RegZero;
            rf_waddr_o = rd;
            jump_flag_o = 1'b0;
          op1_jump_o = `ysyx_24090018_ZeroWord;
          op2_jump_o = `ysyx_24090018_ZeroWord;
        end
        `ysyx_24090018_INST_J_1101111: begin //jal
          op1_o = inst_addr_i;
          op2_o = 32'h4;
          rf_we_o = `ysyx_24090018_WriteDisable;
          rf_waddr_o = rd;
          rf_raddr1_o = `ysyx_24090018_RegZero;
          rf_raddr2_o = `ysyx_24090018_RegZero;
          op1_jump_o = inst_addr_i;
          op2_jump_o = {{12{inst_i[31]}}, inst_i[19:12], inst_i[20], inst_i[30:21], 1'b0};
          jump_flag_o = 1'b1;
        end
        `ysyx_24090018_INST_I_1100111: begin //jalr
          op1_o = inst_addr_i;
          op2_o = 32'h4;
          rf_we_o = `ysyx_24090018_WriteEnable;
          rf_raddr1_o = rs1;
          rf_raddr2_o = `ysyx_24090018_RegZero;
          rf_waddr_o = rd;
          op1_jump_o = rf_rdata1_i;
          op2_jump_o = {{20{inst_i[31]}}, inst_i[31:20]};
          jump_flag_o = 1'b1;
        end
        `ysyx_24090018_INST_U_0110111: begin  //lui
            op1_o = {inst_i[31:12], 12'b0};
            op2_o = 32'h0;
            rf_we_o = `ysyx_24090018_WriteEnable;
            rf_raddr1_o =`ysyx_24090018_RegZero;
            rf_raddr2_o = `ysyx_24090018_RegZero;
            rf_waddr_o = rd;
            jump_flag_o = 1'b0;
          op1_jump_o = `ysyx_24090018_ZeroWord;
          op2_jump_o = `ysyx_24090018_ZeroWord;
        end
         `ysyx_24090018_INST_U_0010111: begin  //auipc
            op1_o = {inst_i[31:12], 12'b0};
            op2_o = inst_addr_i;
            rf_we_o = `ysyx_24090018_WriteEnable;
            rf_raddr1_o =`ysyx_24090018_RegZero;
            rf_raddr2_o = `ysyx_24090018_RegZero;
            rf_waddr_o = rd;
            jump_flag_o = 1'b0;
          op1_jump_o = `ysyx_24090018_ZeroWord;
          op2_jump_o = `ysyx_24090018_ZeroWord;
          end
         default: begin
          rf_we_o = `ysyx_24090018_WriteDisable;
          rf_waddr_o = `ysyx_24090018_RegZero;
          rf_raddr1_o = `ysyx_24090018_RegZero;
          rf_raddr2_o = `ysyx_24090018_RegZero;
          jump_flag_o = 1'b0;
          op1_jump_o = `ysyx_24090018_ZeroWord;
          op2_jump_o = `ysyx_24090018_ZeroWord;
          op1_o = `ysyx_24090018_ZeroWord;
          op2_o = `ysyx_24090018_ZeroWord;
         end

    endcase
  end

  /*
    //rf_we_o selection
    ysyx_24090018_MuxKeyWithDefault #(6,7,1) i0 (rf_we_o, opcode, `ysyx_24090018_WriteDisable, {
        `ysyx_24090018_INST_I, `ysyx_24090018_WriteEnable
        `ysyx_24090018_INST_R, `ysyx_24090018_WriteEnable
        `ysyx_24090018_INST_U, `ysyx_24090018_WriteEnable
        `ysyx_24090018_INST_B, `ysyx_24090018_WriteDisable
        `ysyx_24090018_INST_J, `ysyx_24090018_WriteEnable
        `ysyx_24090018_INST_S, `ysyx_24090018_WriteDisable        
      }); 

    //rf_waddr_o selection
    ysyx_24090018_MuxKeyWithDefault #(6,7,32) i0 (rf_waddr_o, opcode, `ysyx_24090018_RegZero, {
        `ysyx_24090018_INST_I, rd
        `ysyx_24090018_INST_R, rd
        `ysyx_24090018_INST_U, rd
        `ysyx_24090018_INST_B, `ysyx_24090018_RegZero
        `ysyx_24090018_INST_J, rd
        `ysyx_24090018_INST_S, `ysyx_24090018_RegZero 
      }); 

    //rf_raddr1_o selection
    ysyx_24090018_MuxKeyWithDefault #(6,7,32) i0 (rf_raddr1_o, opcode, `ysyx_24090018_RegZero, {
        `ysyx_24090018_INST_I, rs1
        `ysyx_24090018_INST_R, rs1
        `ysyx_24090018_INST_U, `ysyx_24090018_RegZero
        `ysyx_24090018_INST_B, rs1
        `ysyx_24090018_INST_J, rs1
        `ysyx_24090018_INST_S, rs1        
      }); 
    */
endmodule
