BUILD_DIR = $(SRC_DIR)/build
OBJS_DIR = $(BUILD_DIR)/objs
MAIN_PROG = 20141570

.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR) $(SRC_DIR)/../$(MAIN_PROG)
	@echo "Cleaned."

$(addsuffix .o, $(TARGET)): %.o: %.c %.h .mkdir.o
	gcc -c $(basename $@).c -o $(OBJS_DIR)/$(basename $@).o

.PHONY: .mkdir.o
.mkdir.o:
	@mkdir -p $(OBJS_DIR)
