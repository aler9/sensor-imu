RPI_IMAGE = balenalib/raspberry-pi:bullseye
UBUNTU_IMAGE = ubuntu:22.04

.PHONY: $(shell ls)

help:
	@echo "usage: make [action]"
	@echo ""
	@echo "available actions:"
	@echo ""
	@echo "  format    format source files"
	@echo "  lint      run linter"
	@echo "  test      run tests"
	@echo ""

include scripts/*.mk
