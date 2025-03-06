/*************************************************************************
	> File Name: RegisterFile.v
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2024年10月06日 星期日 11时18分04秒
 ************************************************************************/
module ysyx_24090018_RegisterFile #(ADDR_WIDTH = 5, DATA_WIDTH = 32) (
  input clk,
  input [DATA_WIDTH-1:0] wdata,
  input [ADDR_WIDTH-1:0] waddr,
  input wen,

  input [ADDR_WIDTH-1:0] rs1,
  input [ADDR_WIDTH-1:0] rs2,
  output reg [DATA_WIDTH-1:0] rdata1,
  output reg [DATA_WIDTH-1:0] rdata2
);
  reg [DATA_WIDTH-1:0] rf [2**(ADDR_WIDTH-1)-1:0];  //寄存器数量
  always @(posedge clk) begin //0号寄存器永远存0
    rf[0] <= {DATA_WIDTH{1'b0}};
  end

  always @(posedge clk) begin
    if (wen && waddr != 0) rf[waddr[3:0]] <= wdata;
  end

 // assign rdata1 = rf[rs1[3:0]];  //read src1 data
 // assign rdata2 = rf[rs2[3:0]];  //read src2 data
  always @(*) begin
    rdata1 = rf[rs1[3:0]];
    rdata2 = rf[rs2[3:0]];
  end
endmodule
