-include $(YSYX_HOME)/Makefile
include $(NPC_HOME)/scripts/build.mk

override ARGS ?= --log=$(BUILD_DIR)/npc-log.txt

# Command to execute NPC
IMG ?=
NPC_EXEC := $(BINARY) $(ARGS) $(IMG)

run: $(BINARY)
	$(call git_commit, "sim RTL")  #开发跟踪
	$(NPC_EXEC)

gdb: $(BINARY)
	gdb -s $(BINARY) --args $(NPC_EXEC)

#清除fixdep的build和kconfig的build，但是当前还有bug
clean-tools = $(dir $(shell find ./tools -maxdepth 2 -mindepth 2 -name "Makefile"))
$(clean-tools):
	-@$(MAKE) -s -C $@ clean
clean-tools: $(clean-tools)
clean-all: clean distclean clean-tools

.PHONY: run gdb clean-tools clean-all $(clean-tools)
