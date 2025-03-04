ifdef CONFIG_DIFFTEST
DIFF_REF_PATH = $(NEMU_HOME)
DIFF_REF_SO = $(DIFF_REF_PATH)/build/riscv32-nemu-interpreter-so
ARGS_DIFF = --diff=$(DIFF_REF_SO)

$(DIFF_REF_SO):
	$(MAKE) -s -C $(DIFF_REF_PATH)

.PHONY: $(DIFF_REF_SO)
endif
