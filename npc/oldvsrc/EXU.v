/*************************************************************************
	> File Name: EXU.v
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2024年10月07日 星期一 13时57分16秒
 ************************************************************************/
`include "Defines.v"

module ysyx_24090018_EXU #(DATA_WIDTH = 32)(
  input [DATA_WIDTH-1:0] op1_i,
  input [DATA_WIDTH-1:0] op2_i,
  input [DATA_WIDTH-1:0] inst_i,
  input [DATA_WIDTH-1 : 0] inst_addr_i,
  input [DATA_WIDTH-1:0] op1_jump_i,
  input [DATA_WIDTH-1:0] op2_jump_i,


  output reg [DATA_WIDTH-1:0] rf_wdata_o,
  output reg [DATA_WIDTH-1:0] jump_addr_o
);
  wire [6:0] opcode;
  wire [2:0] funct3;
  wire [6:0] funct7;

  assign opcode = inst_i[6:0];
  assign funct3 = inst_i[14:12];
  assign funct7 = inst_i[31:25];

  reg ebreak;
  export "DPI-C" function get_ebreak;
  function get_ebreak;
    output bit finish;
    finish = ebreak;
  endfunction

  export "DPI-C" function get_inst;
  function get_inst;
    output  [31:0] inst;
    inst = inst_i;
  endfunction

  always@(*) begin
    case(opcode)
      `ysyx_24090018_INST_I_0010011: begin
        case(funct3) 
          `ysyx_24090018_funct3_ADDI: begin
             rf_wdata_o = op1_i + op2_i;
          end
          default: begin
             rf_wdata_o = `ysyx_24090018_ZeroWord;
          end
        endcase
      end
      `ysyx_24090018_INST_I_1110011: begin
          case(funct3)
            `ysyx_24090018_funct3_EBREAK_ECALL: begin
                ebreak = inst_i[20] ? 1'b1 : 1'b0;
                rf_wdata_o = `ysyx_24090018_ZeroWord; 
            end
            default: begin
                ebreak = 1'b0;
                rf_wdata_o = `ysyx_24090018_ZeroWord;
             end
          endcase
      end
      `ysyx_24090018_INST_J_1101111,`ysyx_24090018_INST_I_1100111: begin
        jump_addr_o = op1_jump_i + op2_jump_i;
        rf_wdata_o = op1_i + op2_i;
      end
      `ysyx_24090018_INST_U_0010111, `ysyx_24090018_INST_U_0110111: begin //
        rf_wdata_o = op1_i + op2_i;
      end
      default: begin
        rf_wdata_o = `ysyx_24090018_ZeroWord; 
        jump_addr_o = `ysyx_24090018_ZeroWord;
      end
    endcase
  end
endmodule
