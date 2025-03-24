.DEFAULT_GOAL = app

TOPNAME = top

WORK_DIR  = $(shell pwd)
INC_PATH := $(WORK_DIR)/include

VERILATOR = verilator
VERILATOR_CFLAGS += -MMD --build -cc --trace \
				-O2 --x-assign fast --x-initial fast --noassert
VERILATOR_CFLAGS += -Ivsrc

BUILD_DIR = ./build
OBJ_DIR = $(BUILD_DIR)/obj_dir
BINARY = $(BUILD_DIR)/$(TOPNAME)


$(shell mkdir -p $(BUILD_DIR))

# project source
VSRCS = $(shell find $(abspath ./vsrc) -name "*.v")
CSRCS = $(shell find $(abspath ./csrc) -name "*.c" -or -name "*.cc" -or -name "*.cpp")

# rules for verilator
INCFLAGS = $(addprefix -I, $(INC_PATH))
CXXFLAGS += $(INCFLAGS) -DTOP_NAME="\"V$(TOPNAME)\"" -g
# 添加 readline 库到链接标志
LDFLAGS += -lreadline -ldl -pie  # 如果需要，也可以添加 -ldl 和 -pie

#ifneq ($(CONFIG_ITRACE)$(CONFIG_IQUEUE),)
CSRCs += ./csrc/utils/disasm.cc
#CXXFLAGS += $(shell llvm-config --cxxflags) -fPIE
CXXFLAGS += -I/usr/lib/llvm-18/include -std=c++17   -fno-exceptions -funwind-tables -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS -fPIE
LDFLAGS += $(shell llvm-config --libs) #-Wl,--verbose
#endif

app: $(BINARY)

$(BINARY): $(VSRCS) $(CSRCS)# $(NVBOARD_ARCHIVE)
	@rm -rf $(OBJ_DIR)
	$(VERILATOR) $(VERILATOR_CFLAGS) \
		--top-module $(TOPNAME) $^ \
		$(addprefix -CFLAGS , $(CXXFLAGS)) $(addprefix -LDFLAGS , $(LDFLAGS)) \
		--Mdir $(OBJ_DIR) --exe -o $(abspath $(BINARY))

clean:
	-rm -rf $(BUILD_DIR)

.PHONY: app clean
