BUILD_DIR = $(SRC_DIR)/build
OBJS_DIR = $(BUILD_DIR)/objs

.PHONY: clean
clean:
	@rm -rf $(SRC_DIR)/$(BUILD_DIR) $(SRC_DIR)/../$(MAIN_PROG)
	@echo "Cleaned."

$(addsuffix .o, $(TARGET)): %.o: %.c %.h .mkdir.o
	gcc -c $(basename $@).c -o $(OBJS_DIR)/$(basename $@).o

.PHONY: .mkdir.o
.mkdir.o:
	@mkdir -p $(OBJS_DIR)
