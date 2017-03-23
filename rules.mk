BUILD_DIR = build

.PHONY: clean
clean:
	@rm -rf $(SRC_DIR)/$(BUILD_DIR) $(MAIN_PROG)
	@echo "Cleaned."

$(addsuffix .o, $(TARGET)): %.o: .mkdir.o
	gcc -c $(basename $@).c -o $(BUILD_DIR)/$(basename $@).o

.PHONY: .mkdir.o
.mkdir.o:
	@mkdir -p $(SRC_DIR)/$(BUILD_DIR)
