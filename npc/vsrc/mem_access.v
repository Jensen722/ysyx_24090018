/*************************************************************************
	> File Name: /home/jensen/ysyx-workbench/npc/vsrc/Mem.v
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: Fri 07 Mar 2025 02:13:16 PM CST
 ************************************************************************/
`include "defines.v"

module ysyx_24090018_mem_access #(ADDR_WIDTH = 32, DATA_WIDTH = 32)(
  input [ADDR_WIDTH-1 : 0] mem_addr_i,
  input [DATA_WIDTH-1 : 0] mem_wdata_i,
  input MemRead_i,
  input [2:0] MemOp_i,
  input MemWrite_i,

  output [DATA_WIDTH-1 : 0] mem_load_data_o
);

import "DPI-C" function int paddr_read(input int raddr, input int len);
import "DPI-C" function void paddr_write(input int waddr,input int len,  input int wdata, input byte wmask);

  reg [DATA_WIDTH-1 : 0] mem_rdata;
  wire [7:0] wmask;
  wire [7:0] mem_byte_data;
  wire [15:0] mem_halfword_data;

  ysyx_24090018_MuxKey #(4, 2, 8) byte_data_select(
    .out(mem_byte_data),
    .key(mem_addr_i[1:0]),
    .lut({
      2'b00, mem_rdata[7:0],
      2'b01, mem_rdata[15:8],
      2'b10, mem_rdata[23:16],
      2'b11, mem_rdata[31:24]
    })
  );

  ysyx_24090018_MuxKey #(2, 1, 16) halfword_data_sel(
    .out(mem_halfword_data),
    .key(mem_addr_i[1]),
    .lut({
      1'b0, mem_rdata[15:0],
      1'b1, mem_rdata[31:16]
    })
  );

  //select load-data according to MemOp signal
  ysyx_24090018_MuxKeyWithDefault #(5, 3, 32) load_data_sel(
    .out(mem_load_data_o),
    .key(MemOp_i),
    .default_out(32'h0),
    .lut({
      3'b000, {{24{mem_byte_data[7]}}, mem_byte_data},               //lb
      3'b001, {{16{mem_halfword_data[15]}}, mem_halfword_data},      //lh
      3'b010, mem_rdata,                                             //lw
      3'b100, {{24{1'b0}}, mem_byte_data},                           //lbu
      3'b101, {{16{1'b0}}, mem_halfword_data}                        //lhu
    })
  );

  ysyx_24090018_MuxKeyWithDefault #(3, 2, 8) wmask_sel(
    .out(wmask),
    .key(MemOp_i[1:0]),
    .default_out(8'h0),
    .lut({
      2'b00, 8'b00000001,
      2'b01, 8'b00000011,
      2'b10, 8'b00001111
    })
  );

  always @(*) begin
    if(MemRead_i) begin
      mem_rdata = paddr_read(mem_addr_i, 32'h4);
    end
    else begin
      mem_rdata = 0;
    end
  end

  //地址对齐通过内存总线访问内存，每次都是返回32bit
  always @(*) begin
      if (MemWrite_i) begin // 有写请求时
        paddr_write(mem_addr_i, 32'h4, mem_wdata_i, wmask);
      end
  end
endmodule
