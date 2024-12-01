VSRC=$(wildcard ./vsrc/switch.v)
CSRC=$(wildcard ./csrc/sim.cpp)

all:
	@echo "Write this Makefile by your self."

sim:
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	verilator --cc --exe --trace -Wall -j 0 $(VSRC) $(CSRC)
	make -C obj_dir -f Vswitch.mk Vswitch   #also can use --build to make itself
	./obj_dir/Vswitch
	#gtkwave wave.vcd

.PHONY:clean

clean:
	rm -rf obj_dir wave.vcd

include ../Makefile
