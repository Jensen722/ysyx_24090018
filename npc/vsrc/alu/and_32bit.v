/*************************************************************************
	> File Name: and32.v
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2024年12月05日 星期四 18时55分42秒
 ************************************************************************/
module and_32bit(S,A,B);

input  [31:0] A,B;
output [31:0] S;

and and0(S[0], A[0], B[0]);
and and1(S[1], A[1], B[1]);
and and2(S[2], A[2], B[2]);

and and3(S[3], A[3], B[3]);
and and4(S[4], A[4], B[4]);
and and5(S[5], A[5], B[5]);

and and6(S[6], A[6], B[6]);
and and7(S[7], A[7], B[7]);
and and8(S[8], A[8], B[8]);

and and9(S[9], A[9], B[9]);
and and10(S[10], A[10], B[10]);
and and11(S[11], A[11], B[11]);

and and12(S[12], A[12], B[12]);
and and13(S[13], A[13], B[13]);
and and14(S[14], A[14], B[14]);

and and15(S[15], A[15], B[15]);
and and16(S[16], A[16], B[16]);
and and17(S[17], A[17], B[17]);

and and18(S[18], A[18], B[18]);
and and19(S[19], A[19], B[19]);
and and20(S[20], A[20], B[20]);

and and21(S[21], A[21], B[21]);
and and22(S[22], A[22], B[22]);
and and23(S[23], A[23], B[23]);

and and24(S[24], A[24], B[24]);
and and25(S[25], A[25], B[25]);
and and26(S[26], A[26], B[26]);

and and27(S[27], A[27], B[27]);
and and28(S[28], A[28], B[28]);
and and29(S[29], A[29], B[29]);

and and30(S[30], A[30], B[30]);
and and31(S[31], A[31], B[31]);
endmodule
