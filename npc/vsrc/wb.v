/*************************************************************************
	> File Name: /home/jensen/ysyx-workbench/npc/vsrc/wb.v
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: Fri 07 Mar 2025 03:41:32 PM CST
 ************************************************************************/
module ysyx_24090018_wb #(DATA_WIDTH = 32)(
  input RegWrite_i,
  input [DATA_WIDTH-1 : 0] mem_load_data_i,
  input [DATA_WIDTH-1 : 0] alu_out_i,
  input Mem2Reg_i,

  output [DATA_WIDTH-1 : 0] data_wb_o,
  output RegWrite_o
);

  ysyx_24090018_MuxKey #(2, 1, 32) wb_data_sel (
    .out(data_wb_o),
    .key(Mem2Reg_i),
    .lut({
      1'b0, alu_out_i,
      1'b1, mem_load_data_i
    })
  );

  assign RegWrite_o = RegWrite_i;

endmodule
