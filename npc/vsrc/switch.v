/*************************************************************************
	> File Name: switch.v
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2024年08月17日 星期六 11时27分41秒
 ************************************************************************/
module switch(
  input a,
  input b,
  output f
);
  assign f = a ^ b;
endmodule
