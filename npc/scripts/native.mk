-include $(NPC_HOME)/../Makefile
include $(NPC_HOME)/scripts/build.mk

include $(NPC_HOME)/tools/difftest.mk
# Some convenient rules

override ARGS ?= --log=$(BUILD_DIR)/npc-log.txt
override ARGS += $(ARGS_DIFF)

# Command to execute NPC
IMG ?=
NPC_EXEC := $(BIN) $(ARGS) $(IMG)

run-env: $(BIN) $(DIFF_REF_SO)

run: run-env
	$(call git_commit, "sim RTL")
	$(NPC_EXEC)

.PHONY: run run-env
