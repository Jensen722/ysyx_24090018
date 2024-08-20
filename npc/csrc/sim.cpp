/*************************************************************************
	> File Name: sim.cpp
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2024年08月19日 星期一 09时51分38秒
 ************************************************************************/
 #include <stdio.h>
 #include <stdlib.h>
 #include <assert.h>
 #include "Vswitch.h"
 #include "verilated_vcd_c.h" 
#include "verilated.h"
 int main(int argc, char** argv){
 
   VerilatedContext* contextp = new VerilatedContext;                        
   contextp->commandArgs(argc, argv);
   Vswitch* top = new Vswitch{contextp};
   VerilatedVcdC *tfp = new VerilatedVcdC;
   contextp->traceEverOn(true);
   top->trace(tfp,0);
   tfp->open("wave.vcd");
 
   while (!contextp->gotFinish()){
     int a = rand() & 1;
     int b = rand() & 1;
     top->a = a;
     top->b = b;
     top->eval();
     printf("a = %d, b = %d, f = %d\n", a, b, top->f);
     assert(top->f == (a ^ b));

     tfp->dump(contextp->time());
     contextp->timeInc(1);
   }
 
   delete top;
   delete contextp;
   return 0;
 
 }
