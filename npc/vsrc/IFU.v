/*************************************************************************
	> File Name: IFU.v
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2025年03月05日 星期三 15时11分54秒
 ************************************************************************/
`include "defines.v"

module ysyx_24090018_IFU #(ADDR_WIDTH = 32, DATA_WIDTH = 32)(
  input clk,
  input [ADDR_WIDTH-1 : 0] pc_i,
  output reg [DATA_WIDTH-1 : 0] inst_o
);

import "DPI-C" function int paddr_read(input int raddr, input int len);
always @(*) begin
    inst_o = paddr_read(pc_i, 32'h4);
end

endmodule
