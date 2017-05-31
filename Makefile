SRC_DIR = src
WARN_DIR = ../warn-make/

all:
	make -C $(SRC_DIR) build

debug:
	make -C $(SRC_DIR) debug

warn:
	make -C $(SRC_DIR) warn

include $(SRC_DIR)/rules.mk
