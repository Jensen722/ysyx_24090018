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

  output reg [DATA_WIDTH-1:0] rf_wdata_o
);
  wire [6:0] opcode;
  wire [2:0] funct3;
  wire [6:0] funct7;

  assign opcode = inst_i[6:0];
  assign funct3 = inst_i[14:12];
  assign funct7 = inst_i[31:25];

  always@(*) begin
    case(opcode)
      `ysyx_24090018_INST_I: begin
        case(funct3) 
          `ysyx_24090018_ADDI: begin
             rf_wdata_o = op1_i + op2_i;
          end
          default: begin
             rf_wdata_o = `ysyx_24090018_ZeroWord;
          end
        endcase
      end

      default: begin
        rf_wdata_o = `ysyx_24090018_ZeroWord; 
      end
    endcase
  end
endmodule
