BUILD_DIR = $(SRC_DIR)/build
OBJS_DIR = $(BUILD_DIR)/objs
MAIN_PROG = main_prog

.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR) $(SRC_DIR)/../$(MAIN_PROG)
	@echo "Cleaned."

$(addsuffix .o, $(TARGET)): %.o: %.c %.h .mkdir.o
	gcc -c $(CC_FLAGS) $(basename $@).c -o $(OBJS_DIR)/$(notdir $(basename $@)).o

.PHONY: .mkdir.o
.mkdir.o:
	@mkdir -p $(OBJS_DIR)
